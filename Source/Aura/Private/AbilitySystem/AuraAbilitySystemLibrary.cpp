// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/MenuWidgetController.h"

UMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(UObject* WorldContext)
{
	if(APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(WorldContext,0)))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC,PS,AS,ASC);
			UMenuWidgetController* MenuWidgetController = AuraHUD->GetMenuWidgetController(WidgetControllerParams);
			
			return MenuWidgetController;
		}
	}

	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(UObject* WorldContext)
{
	if(APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(WorldContext,0)))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC,PS,AS,ASC);
			USpellMenuWidgetController* SpellMenuWidgetController = AuraHUD->GetSpellMenuWidgetController(WidgetControllerParams);
			
			return SpellMenuWidgetController;
		}
	}

	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(UObject* WorldContext, ECharacterClass CharacterClass,float level, UAbilitySystemComponent* ASC)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
 	UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	const AActor* AvatarActor = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(AvatarActor);
	ASC ->ApplyGameplayEffectSpecToSelf(
		*ASC->MakeOutgoingSpec(CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass).PrimaryAttributes,level,PrimaryContextHandle).Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	ASC ->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,level,SecondaryContextHandle).Data.Get());

	FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	ASC ->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,level,VitalContextHandle).Data.Get());
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(UObject* WorldContext, TArray<AActor*>& OverlapActors,
	const TArray<AActor*>& ActorsToIgnore, float Radius, FVector SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
	}

	for (const FOverlapResult& Overlap : Overlaps)
	{
		if(Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
		{
			OverlapActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
		}
	}
}

bool UAuraAbilitySystemLibrary::GetIsBlocked(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetIsBlock();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetIsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlocked(bool IsBlocked, FGameplayEffectContextHandle& EffectContextHandle)
{
	if(FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsBlock(IsBlocked);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(bool IsCriticalHit, FGameplayEffectContextHandle& EffectContextHandle)
{
	if(FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsCriticalHit(IsCriticalHit);
	}
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(UObject* WorldContext)
{
	if(APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(WorldContext,0)))
	{
		AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
		return PS->AbilityInfo;
	}

	return nullptr;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}