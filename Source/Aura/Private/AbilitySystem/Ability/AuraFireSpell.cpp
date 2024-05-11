// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraFireSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Tasks/GetMouseData.h"
#include "AttributesTagsInfo.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"

void UAuraFireSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
}

void UAuraFireSpell::SpawnProjectile(const FVector& MouseLocation, const FGameplayTag& SocketTag)
{
	const bool bOnServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bOnServer) return;

	

	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		FTransform Transform;
		FVector SocketLocation = CombatInterface->Execute_GetWeaponTipSocketLocation(GetAvatarActorFromActorInfo(),SocketTag);
		FRotator SpellRotator = (MouseLocation - SocketLocation).Rotation();
		SpellRotator.Pitch = 0.f;
		Transform.SetLocation(SocketLocation);
		Transform.SetRotation(SpellRotator.Quaternion());
	
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,Transform,GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->SetSpawnActor(GetAvatarActorFromActorInfo());

		if(UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
		{
			FGameplayEffectContextHandle GameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
			GameplayEffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
			
			const FGameplayEffectSpecHandle GameplayEffectSpecHandle =
				AbilitySystemComponent->MakeOutgoingSpec(DamageEffect,GetAbilityLevel(),GameplayEffectContextHandle);
			
			Projectile->GameplayEffectSpecHandle = GameplayEffectSpecHandle;
			const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpecHandle,GameplayTags.Damage,DamageNumber);
		}
		
		Projectile->FinishSpawning(Transform);
	}
}

void UAuraFireSpell::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetWeaponTipSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	
	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumProjectiles = NumProjectiles;
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->SetSpawnActor(GetAvatarActorFromActorInfo());
		
		if(UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
		{
			FGameplayEffectContextHandle GameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
			GameplayEffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
			
			const FGameplayEffectSpecHandle GameplayEffectSpecHandle =
				AbilitySystemComponent->MakeOutgoingSpec(DamageEffect,GetAbilityLevel(),GameplayEffectContextHandle);
			
			Projectile->GameplayEffectSpecHandle = GameplayEffectSpecHandle;
			const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpecHandle,GameplayTags.Damage,DamageNumber);
		}
		
		
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovementComponent->bIsHomingProjectile = bLaunchHomingProjectiles;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}