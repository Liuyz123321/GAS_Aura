// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AttributesTagsInfo.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::OnASinfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::EffectApplied);
}


void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& Tag)
{
	if(!Tag.IsValid()) return;

	for(FGameplayAbilitySpec& Ability: GetActivatableAbilities())
	{
		if(Ability.DynamicAbilityTags.HasTagExact(Tag))
		{
			AbilitySpecInputReleased(Ability);
			
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& Tag)
{
	if(!Tag.IsValid()) return;

	for(FGameplayAbilitySpec& Ability: GetActivatableAbilities())
	{
		if(Ability.DynamicAbilityTags.HasTagExact(Tag))
		{
			AbilitySpecInputPressed(Ability);	
			if(!Ability.IsActive())
			{
				TryActivateAbility(Ability.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	
	for(const TSubclassOf<UGameplayAbility> Ability: StartUpAbilities)
	{
		FGameplayAbilitySpec GameplayAbilitySpec(Ability,1);
		if(UAuraGameplayAbility* GameplayAbility = Cast<UAuraGameplayAbility>(GameplayAbilitySpec.Ability))
		{
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(GameplayAbility->StartUpTag);
			GiveAbility(GameplayAbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);

	EffectTags.Broadcast(TagContainer);
	
}
