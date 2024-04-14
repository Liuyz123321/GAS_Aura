// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AttributesTagsInfo.h"
#include "ShaderPrintParameters.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void AAuraEnemy::OnTagAdd(const FGameplayTag GameplayTag, int32 NewCount)
{
	bHitReact = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReact?0:BaseWalkSpeed;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(5.f);
	Super::Die();
	Dissolve();
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

