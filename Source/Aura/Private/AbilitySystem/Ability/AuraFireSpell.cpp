// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraFireSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Tasks/GetMouseData.h"
#include "AttributesTagsInfo.h"
#include "Actor/AuraProjectile.h"
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
