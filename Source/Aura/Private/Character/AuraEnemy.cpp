// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AttributesTagsInfo.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!HasAuthority()) return;
	EnemyAIController = Cast<AAuraAIController>(NewController);
	EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	EnemyAIController->RunBehaviorTree(BehaviorTree);
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitReacting"),false);
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsRangeAttack"),EnemyType != ECharacterClass::Warrior);
}

void AAuraEnemy::Highlight()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);
	weapon->SetRenderCustomDepth(true);
	weapon->SetCustomDepthStencilValue(250);
}

void AAuraEnemy::unHighlight()
{
	GetMesh()->SetRenderCustomDepth(false);
	weapon->SetRenderCustomDepth(false);
	
}

int32 AAuraEnemy::getLevel()
{
	return level;
}

void AAuraEnemy::InitDefaultAttributes()
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this,EnemyType,level,AbilitySystemComponent);
}

FMontageTag AAuraEnemy::GetMeleeAttackAnimMontageAndTag_Implementation()
{
	int MontageTagNum = MontageTags.Num();
	int index = FMath::RandRange(0,MontageTagNum-1);
	return MontageTags[index];
}

AActor* AAuraEnemy::GetTargetActor_Implementation() const
{
	return TargetActor;
}

void AAuraEnemy::SetTargetActor_Implementation(AActor* TarActor)
{
	TargetActor = TarActor;
}

void AAuraEnemy::OnTagAdd(const FGameplayTag GameplayTag, int32 NewCount)
{
	bHitReact = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReact? 0:BaseWalkSpeed;
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitReacting"),bHitReact);
}

void AAuraEnemy::Die()
{
	SetLifeSpan(5.f);
	Super::Die();
	Dissolve();
}

void AAuraEnemy::AddAbilities()
{
	Super::AddAbilities();

	UCharacterClassInfo* CharacterClassInfo = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this))->CharacterClassInfo;
	FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(EnemyType);
	for(TSubclassOf<UGameplayAbility> Ability:CharacterClassDefaultInfo.StartUpAbilities)
	{
		Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AddAbility(Ability,level);
	}
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitASinfo();

	AddAbilities();

	const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet);
	
	if(AuraAS)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this,AuraAS](const FOnAttributeChangeData& Data)
			{
				ChangeEnemyProgressBar.Broadcast(Data.NewValue/AuraAS->GetMaxHealth());
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
		[this,AuraAS](const FOnAttributeChangeData& Data)
			{
				ChangeEnemyProgressBar.Broadcast(AuraAS->GetHealth()/Data.NewValue);
			}
		);
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effect_HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AAuraEnemy::OnTagAdd);
	}

	ChangeEnemyProgressBar.Broadcast(AuraAS->GetHealth()/AuraAS->GetMaxHealth());
}

void AAuraEnemy::Dissolve()
{
	if(IsValid(MeshDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(MeshDissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0,MaterialInstanceDynamic);
		MeshDissolveTimeLine(MaterialInstanceDynamic);
	}
	if(IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance,this);
		weapon->SetMaterial(0,MaterialInstanceDynamic);
		WeaponDissolveTimeLine(MaterialInstanceDynamic);
	}
}

void AAuraEnemy::InitASinfo()
{
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->OnASinfoSet();

	InitDefaultAttributes();
}

