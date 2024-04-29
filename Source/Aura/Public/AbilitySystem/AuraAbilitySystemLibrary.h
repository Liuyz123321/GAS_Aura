// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilityInfo;
class UAbilitySystemComponent;
enum class ECharacterClass;
class UMenuWidgetController;
/**
 * 
 */
UCLASS(BlueprintType)
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintPure, Category= "AbilitySystem|WidgetController",meta=(DefaultToSelf = "WorldContext"))
	static UMenuWidgetController* GetAttributeMenuWidgetController(UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category= "AbilitySystem|WidgetController",meta=(DefaultToSelf = "WorldContext"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(UObject* WorldContext);

	static void InitializeDefaultAttributes(UObject* WorldContext,ECharacterClass CharacterClass,float level,UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable)
	static void GetLivePlayersWithinRadius(UObject* WorldContext,TArray<AActor*>& OverlapActors, const TArray<AActor*>& ActorsToIgnore, float Radius, FVector SphereOrigin);

	UFUNCTION(BlueprintPure)
	static bool GetIsBlocked(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure)
	static bool GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetIsBlocked(bool IsBlocked, FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetIsCriticalHit(bool IsCriticalHit, FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static UAbilityInfo* GetAbilityInfo(UObject* WorldContext);
};
