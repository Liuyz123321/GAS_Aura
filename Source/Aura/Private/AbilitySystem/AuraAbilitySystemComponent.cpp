// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AttributesTagsInfo.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Interaction/PlayerInterface.h"

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

void UAuraAbilitySystemComponent::AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities,
	const TArray<TSubclassOf<UGameplayAbility>>& PassiveStartUpAbilities)
{
	for(const TSubclassOf<UGameplayAbility> Ability: StartUpAbilities)
	{
		FGameplayAbilitySpec GameplayAbilitySpec(Ability,1);
		if(UAuraGameplayAbility* GameplayAbility = Cast<UAuraGameplayAbility>(GameplayAbilitySpec.Ability))
		{
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(GameplayAbility->StartUpTag);
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_State_Equipped);
			GiveAbility(GameplayAbilitySpec);
		}
	}

	for(const TSubclassOf<UGameplayAbility> Ability: PassiveStartUpAbilities)
	{
		FGameplayAbilitySpec GameplayAbilitySpec(Ability,1);
		GiveAbilityAndActivateOnce(GameplayAbilitySpec);
	}

	AbilitiesGiven.Broadcast();
	bAbilitiesGiven = true;
}

void UAuraAbilitySystemComponent::AddAbility(const TSubclassOf<UGameplayAbility>& StartUpAbility, int level)
{
	FGameplayAbilitySpec GameplayAbilitySpec(StartUpAbility,level);
	if(const UAuraGameplayAbility* GameplayAbility = Cast<UAuraGameplayAbility>(GameplayAbilitySpec.Ability))
	{
		GameplayAbilitySpec.DynamicAbilityTags.AddTag(GameplayAbility->StartUpTag);
		GameplayAbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_State_Lock);
		GiveAbility(GameplayAbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(FForEachAbility Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for(const FGameplayAbilitySpec& Spec:GetActivatableAbilities())
	{
		if(!Delegate.ExecuteIfBound(Spec))
		{
			UE_LOG(LogTemp, Error, TEXT("Delegate Not Bound!"));
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::FindAbilityTagForSpec(const FGameplayAbilitySpec& Spec)
{
	if(Spec.Ability)
	{
		for(FGameplayTag Tag:Spec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}

	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.State"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::SpendSpellPoint(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddSpellAward(GetAvatarActor(), -1);
		}
		
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		if (Status.MatchesTagExact(GameplayTags.Abilities_State_Unlock))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_State_Unlock);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_State_Ready);
			Status = GameplayTags.Abilities_State_Ready;
		}
		else if (Status.MatchesTagExact(GameplayTags.Abilities_State_Equipped) || Status.MatchesTagExact(GameplayTags.Abilities_State_Ready))
		{
			AbilitySpec->Level += 1;
		}
		AbilityStatusChanged.Broadcast(AbilityTag,FAuraGameplayTags::Get().Abilities_State_Unlock, 1);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(int newLevel)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAbilityDefaultInfo& Info : AbilityInfo->AbilityDefaultInfos)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (newLevel < Info.RequireLevel) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_State_Unlock);
			GiveAbility(AbilitySpec);
			// MarkAbilitySpecDirty(AbilitySpec);
			AbilityStatusChanged.Broadcast(Info.AbilityTag,FAuraGameplayTags::Get().Abilities_State_Unlock, 1);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::FindInputTagForSpec(const FGameplayAbilitySpec& Spec)
{
	for(FGameplayTag Tag:Spec.DynamicAbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::FindStateTagForSpec(const FGameplayAbilitySpec& Spec)
{
	for(FGameplayTag Tag:Spec.DynamicAbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.State"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& Tag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if(!IPlayerInterface::Execute_IsNoAttributePoints(GetAvatarActor()))
			ServerUpgradeAttribute(Tag);
	}
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& Tag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (tag.MatchesTag(Tag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& Tag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		FGameplayEventData Payload;
		Payload.EventTag = Tag;
		Payload.EventMagnitude = 1.0f;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),Tag,Payload);

		IPlayerInterface::Execute_AddAttributeAward(GetAvatarActor(),-1);
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);

	EffectTags.Broadcast(TagContainer);
	
}
