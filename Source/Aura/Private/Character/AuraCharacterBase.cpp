

#include "Character\AuraCharacterBase.h"

#include "AttributesTagsInfo.h"
#include "ShaderPrintParameters.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


AACharacter::AACharacter()
{

	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	weapon = CreateDefaultSubobject<USkeletalMeshComponent>("weapon");
	weapon->SetupAttachment(GetMesh(),"WeaponHandSocket");
	weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AACharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AACharacter::GetAtrribueSet()
{
	return AttributeSet;
}

FVector AACharacter::GetWeaponTipSocketLocation_Implementation(const FGameplayTag& GameplayTag) 
{
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	if(GameplayTag.MatchesTagExact(AuraGameplayTags.Montage_Attack_Weapon) && IsValid(weapon))
	{
		return weapon->GetSocketLocation(WeaponTipSocketName);
	}
	
	if(GameplayTag.MatchesTagExact(AuraGameplayTags.Montage_Attack_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	
	if(GameplayTag.MatchesTagExact(AuraGameplayTags.Montage_Attack_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}

	return FVector();
}


UAnimMontage* AACharacter::GetHitReactAnimMontage_Implementation()
{
	return HitReactMontage;
}

TArray<FMontageTag> AACharacter::GetMontageTags_Implementation()
{
	return MontageTags;
}

bool AACharacter::IsDead_Implementation()
{
	return bIsDead;
}

AActor* AACharacter::GetAvatar_Implementation()
{
	return this;
}

UNiagaraSystem* AACharacter::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

void AACharacter::Die()
{
	// InRule（EDetachmentRule）：指定解除连接规则的枚举值。它定义了解除连接时对对象位置和旋转的处理方式。可能的枚举值包括
	// `KeepRelativeTransform（默认值）：对象将保持相对位置和旋转不变。
	// `KeepWorldTransform：对象将保持在世界空间中的绝对位置和旋转不变。
	// `KeepRelativeTransformIncludingScale：对象将保持相对位置、旋转和缩放不变。
	// `KeepWorldTransformIncludingScale：对象将保持在世界空间中的绝对位置、旋转和缩放不变。
	// `SnapToTargetNotIncludingScale：对象将与目标位置和旋转对齐，但不考虑缩放。
	// `SnapToTargetIncludingScale：对象将与目标位置、旋转和缩放对齐。
	// bInCallModify（bool）：一个布尔值，指示是否调用修改函数。如果设置为 true，则会调用对象的 Modify() 函数，以确保解除连接操作的修改是可以撤销的。
	UGameplayStatics::PlaySoundAtLocation(this,DeathSound,GetActorLocation(),GetActorRotation());
	
	weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	weapon->SetSimulatePhysics(true);
	weapon->SetEnableGravity(true);
	weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsDead = true;
}


// Called when the game sta rts  or when spawned
void AACharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void AACharacter::applyEffect(TSubclassOf<UGameplayEffect> effect, float level) const
{
	check(GetAbilitySystemComponent());
	check(SetPrimaryAttributes);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(effect,level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AACharacter::InitDefaultAttributes()
{
	applyEffect(SetPrimaryAttributes,1);
	applyEffect(SetSecondaryAttributes,1);
	applyEffect(SetVitalAttributes,1);
}

void AACharacter::AddAbilities()
{
	UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if(!HasAuthority()) return;

	ASC->AddAbilities(StartUpAbilities, PassiveStartUpAbilities);
}

void AACharacter::Dissolve()
{
}

void AACharacter::InitASinfo()
{
}

