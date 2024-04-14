// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectTags, const FGameplayTagContainer&)
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnASinfoSet();

	FEffectTags EffectTags;
	
	void AbilityInputTagReleased(const FGameplayTag& Tag);
	void AbilityInputTagHeld(const FGameplayTag& Tag);
	
	void AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities);
protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
