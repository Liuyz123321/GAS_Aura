// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/ListenForCoolDown.h"


UListenForCoolDown* UListenForCoolDown::WaitForCoolDownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                              FGameplayTag InCoolDownTag)
{
	UListenForCoolDown* ListenForCoolDown = NewObject<UListenForCoolDown>();
	ListenForCoolDown->AbilitySystemComponent = AbilitySystemComponent;
	ListenForCoolDown->CoolDownTag = InCoolDownTag;

	if(!IsValid(AbilitySystemComponent) || !InCoolDownTag.IsValid())
	{
		ListenForCoolDown->EndTask();
		return nullptr;
	}

	AbilitySystemComponent->RegisterGameplayTagEvent(InCoolDownTag,EGameplayTagEventType::NewOrRemoved).AddUObject(ListenForCoolDown,&UListenForCoolDown::OnCoolDownChange);

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(ListenForCoolDown,&UListenForCoolDown::OnEffectAdded);
	
	return ListenForCoolDown;
}

void UListenForCoolDown::EndTask()
{
	if(!IsValid(AbilitySystemComponent)) return;

	AbilitySystemComponent->RegisterGameplayTagEvent(CoolDownTag,EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UListenForCoolDown::OnCoolDownChange(const FGameplayTag InCoolDownTag, int NewCount)
{
	if(NewCount==0)
	{
		OnCoolDownEnd.Broadcast(0.0f);
	}
}

void UListenForCoolDown::OnEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GameplayEffectSpec,
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer AssetTags;
	GameplayEffectSpec.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	GameplayEffectSpec.GetAllGrantedTags(GrantedTags);

	if(AssetTags.HasTagExact(CoolDownTag) || GrantedTags.HasTagExact(CoolDownTag))
	{
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CoolDownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(Query);

		if(TimesRemaining.Num() > 0)
		{
			float TimeRemaining = TimesRemaining[0];
			OnCoolDownStart.Broadcast(TimeRemaining);
		}
	}
}
