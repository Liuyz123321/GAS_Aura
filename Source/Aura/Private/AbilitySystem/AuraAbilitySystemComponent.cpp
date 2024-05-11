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

FGameplayTag UAuraAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

bool UAuraAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, Slot))
		{
			return false;
		}
	}
	return true;
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	return Spec.DynamicAbilityTags.HasTagExact(Slot);
}

bool UAuraAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& Spec)
{
	return Spec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Input")));
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Slot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& Spec)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(Spec);
	const FAbilityDefaultInfo Info = AbilityInfo->FindInfoByTag(AbilityTag);
	const FGameplayTag AbilityType = Info.AbilityType;
	return AbilityType.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Passive);
}

void UAuraAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.DynamicAbilityTags.AddTag(Slot);
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag, bool bActivate)
{
	ActivatePassiveEffect.Broadcast(AbilityTag, bActivate);
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
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

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec);

		const bool bStatusValid = Status == GameplayTags.Abilities_State_Equipped || Status == GameplayTags.Abilities_State_Ready;
		if (bStatusValid)
		{

			// Handle activation/deactivation for passive abilities

			if (!SlotIsEmpty(Slot)) // There is an ability in this slot already. Deactivate and clear its slot.
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot(Slot);
				if (SpecWithSlot)
				{
					// is that ability the same as this ability? If so, we can return early.
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)))
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Abilities_State_Equipped, Slot, PrevSlot);
						return;
					}

					if (IsPassiveAbility(*SpecWithSlot))
					{
						MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*SpecWithSlot), false);
						DeactivatePassiveAbility.Broadcast(GetAbilityTagFromSpec(*SpecWithSlot));
					}

					ClearSlot(SpecWithSlot);
				}
			}

			if (!AbilityHasAnySlot(*AbilitySpec)) // Ability doesn't yet have a slot (it's not active)
			{
				if (IsPassiveAbility(*AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivatePassiveEffect(AbilityTag, true);
				}
				AbilitySpec->DynamicAbilityTags.RemoveTag(GetStatusFromSpec(*AbilitySpec));
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_State_Equipped);
			}
			AssignSlotToAbility(*AbilitySpec, Slot);
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_State_Equipped, Slot, PrevSlot);
	}
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
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
