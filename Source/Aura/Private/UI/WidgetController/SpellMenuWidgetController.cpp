// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet *as = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AbilitySystemComponent);
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	UAbilityInfo* AbilityInfo = AuraPlayerState->AbilityInfo;

	check(AbilityInfo);

	//OnSpellPointsChangeDelegate
	AuraPlayerState->OnSpellPointsChangeDelegate.AddLambda([this](int NewPoints)
	{
		OnSpellPointsChangedDelegate.Broadcast(NewPoints);
		CurrentSpellPoints = NewPoints;

		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
		FString Description;
		FString NextLevelDescription;
		// AuraASC->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
	});

	AuraASC->AbilityStatusChanged.AddLambda([this,AuraASC,AbilityInfo](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			// AuraASC->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		}
			
		if (AbilityInfo)
		{
			FAbilityDefaultInfo Info = AbilityInfo->FindInfoByTag(AbilityTag);
			Info.StateTag = StatusTag;
			AbilityInfosDelegate.Broadcast(Info);
		}
	});
}


void USpellMenuWidgetController::BroadcastInitialValues()
{
	OnSpellPointsChangedDelegate.Broadcast(Cast<AAuraPlayerState>(PlayerState)->GetSpellPoints());

	BroadcastAbilityInfo();
	
	BindCallbacksToDependencies();
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	AAuraPlayerState* AuraPS = Cast<AAuraPlayerState>(PlayerState);
		// if (bWaitingForEquipSelection)
		// {
		// 	const FGameplayTag SelectedAbilityType = AbilityInfo->FindInfoByTag(AbilityTag).AbilityType;
		// 	StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		// 	bWaitingForEquipSelection = false;
		// }
		
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		const int32 SpellPoints = AuraPS->GetSpellPoints();
		FGameplayTag AbilityStatus;	
		
		const bool bTagValid = AbilityTag.IsValid();
		const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
		const FGameplayAbilitySpec* AbilitySpec = AuraASC ->GetSpecFromAbilityTag(AbilityTag);
		const bool bSpecValid = AbilitySpec != nullptr;
		if (!bTagValid || bTagNone || !bSpecValid)
		{
			AbilityStatus = GameplayTags.Abilities_State_Lock;
		}
		else
		{
			AbilityStatus = AuraASC->GetStatusFromSpec(*AbilitySpec);
		}
	
		SelectedAbility.Ability = AbilityTag;
		SelectedAbility.Status = AbilityStatus;
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
		FString Description;
		FString NextLevelDescription;
		// AuraASC->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->SpendSpellPoint(SelectedAbility.Ability);
		BroadcastAbilityInfo();
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	// AAuraPlayerState* AuraPS = Cast<AAuraPlayerState>(PlayerState);
	// UAbilityInfo* AbilityInfo = AuraPS->AbilityInfo;
	// UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	//
	// const FGameplayTag AbilityType = AbilityInfo->FindInfoByTag(SelectedAbility.Ability).AbilityType;
	//
	// WaitForEquipDelegate.Broadcast(AbilityType);
	// bWaitingForEquipSelection = true;
	//
	// const FGameplayTag SelectedStatus = AuraASC->GetStatusFromAbilityTag(SelectedAbility.Ability);
	// if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_State_Equipped))
	// {
	// 	SelectedSlot = AuraASC->GetSlotFromAbilityTag(SelectedAbility.Ability);
	// }
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
}

void USpellMenuWidgetController::BroadcastAbilityInfo()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	UAbilityInfo* AbilityInfo = AuraPlayerState->AbilityInfo;
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	
	if (!AuraASC->bAbilitiesGiven) return;
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this,AbilityInfo,AuraASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAbilityDefaultInfo Info = AbilityInfo->FindInfoByTag(AuraASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = AuraASC->FindInputTagForSpec(AbilitySpec);
		Info.StateTag = AuraASC->GetStatusFromSpec(AbilitySpec);
		AbilityInfosDelegate.Broadcast(Info);
	});
	AuraASC->ForEachAbility(BroadcastDelegate);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                     bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
		bShouldEnableSpellPointsButton = false;
		bShouldEnableEquipButton = false;
		if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_State_Equipped))
		{
			bShouldEnableEquipButton = true;
			if (SpellPoints > 0)
			{
				bShouldEnableSpellPointsButton = true;
			}
		}
		else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_State_Unlock))
		{
			if (SpellPoints > 0)
			{
				bShouldEnableSpellPointsButton = true;
			}
		}
		else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_State_Ready))
		{
			bShouldEnableEquipButton = true;
			if (SpellPoints > 0)
			{
				bShouldEnableSpellPointsButton = true;
			}
		}
}

//
// void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
// {
// 	if (bWaitingForEquipSelection)
// 	{
// 		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
// 		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
// 		bWaitingForEquipSelection = false;
// 	}
// 	
// 	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();	
// 	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
// 	FGameplayTag AbilityStatus;	
// 	
// 	const bool bTagValid = AbilityTag.IsValid();
// 	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
// 	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
// 	const bool bSpecValid = AbilitySpec != nullptr;
// 	if (!bTagValid || bTagNone || !bSpecValid)
// 	{
// 		AbilityStatus = GameplayTags.Abilities_Status_Locked;
// 	}
// 	else
// 	{
// 		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
// 	}
//
// 	SelectedAbility.Ability = AbilityTag;
// 	SelectedAbility.Status = AbilityStatus;
// 	bool bEnableSpendPoints = false;
// 	bool bEnableEquip = false;
// 	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
// 	FString Description;
// 	FString NextLevelDescription;
// 	GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
// 	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
// }
//
// void USpellMenuWidgetController::SpendPointButtonPressed()
// {
// 	if (GetAuraASC())
// 	{
// 		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
// 	}
// }
//
// void USpellMenuWidgetController::GlobeDeselect()
// {
// 	if (bWaitingForEquipSelection)
// 	{
// 		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
// 		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
// 		bWaitingForEquipSelection = false;
// 	}
// 	
// 	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
// 	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;
//
// 	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
// }
//
// void USpellMenuWidgetController::EquipButtonPressed()
// {
// 	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
//
// 	WaitForEquipDelegate.Broadcast(AbilityType);
// 	bWaitingForEquipSelection = true;
//
// 	const FGameplayTag SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
// 	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
// 	{
// 		SelectedSlot = GetAuraASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
// 	}
// }
//
// void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
// {
// 	if (!bWaitingForEquipSelection) return;
// 	// Check selected ability against the slot's ability type.
// 	// (don't equip an offensive spell in a passive slot and vice versa)
// 	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
// 	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;
//
// 	GetAuraASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
// }
//
// void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
// {
// 	bWaitingForEquipSelection = false;
//
// 	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
// 	
// 	FAuraAbilityInfo LastSlotInfo;
// 	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
// 	LastSlotInfo.InputTag = PreviousSlot;
// 	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
// 	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
// 	AbilityInfoDelegate.Broadcast(LastSlotInfo);
//
// 	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
// 	Info.StatusTag = Status;
// 	Info.InputTag = Slot;
// 	AbilityInfoDelegate.Broadcast(Info);
//
// 	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
// 	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
// 	GlobeDeselect();
// }
//
// void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
// {
// 	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
//
// 	bShouldEnableSpellPointsButton = false;
// 	bShouldEnableEquipButton = false;
// 	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
// 	{
// 		bShouldEnableEquipButton = true;
// 		if (SpellPoints > 0)
// 		{
// 			bShouldEnableSpellPointsButton = true;
// 		}
// 	}
// 	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
// 	{
// 		if (SpellPoints > 0)
// 		{
// 			bShouldEnableSpellPointsButton = true;
// 		}
// 	}
// 	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
// 	{
// 		bShouldEnableEquipButton = true;
// 		if (SpellPoints > 0)
// 		{
// 			bShouldEnableSpellPointsButton = true;
// 		}
// 	}
// }
