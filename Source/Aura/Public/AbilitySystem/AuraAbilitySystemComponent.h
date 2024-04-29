// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectTags, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChangedSignature,FGameplayTag,FGameplayTag,int);
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
	FAbilitiesGiven AbilitiesGiven;

	bool bAbilitiesGiven = false;
	
	void AbilityInputTagReleased(const FGameplayTag& Tag);
	void AbilityInputTagHeld(const FGameplayTag& Tag);
	
	void AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities,const TArray<TSubclassOf<UGameplayAbility>>& PassiveStartUpAbilities);
	
	void AddAbility(const TSubclassOf<UGameplayAbility>& StartUpAbility, int level);

	void ForEachAbility(FForEachAbility Delegate);

	static FGameplayTag FindAbilityTagForSpec(const FGameplayAbilitySpec& Spec);

	FGameplayTag FindInputTagForSpec(const FGameplayAbilitySpec& Spec);
	
	FGameplayTag FindStateTagForSpec(const FGameplayAbilitySpec& Spec);

	void UpgradeAttribute(const FGameplayTag& Tag);

	FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	
	FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	void SpendSpellPoint(const FGameplayTag& AbilityTag);

	void UpdateAbilityStatuses(int newLevel);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& Tag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& Tag);
	
	FAbilityStatusChangedSignature AbilityStatusChanged;
	
protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
