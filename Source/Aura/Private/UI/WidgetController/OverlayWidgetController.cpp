// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet *as = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(as->GetHealth());
	OnMaxHealthChanged.Broadcast(as->GetMaxHealth());
	OnManaChanged.Broadcast(as->GetMana());
	OnMaxManaChanged.Broadcast(as->GetMaxMana());

	BindCallbacksToDependencies();
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet *as = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(as->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(as->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(as->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(as->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
		);

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectTags.AddLambda(
		[this](const FGameplayTagContainer & Tags)
		{
			for (auto tag:Tags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if(tag.MatchesTag(MessageTag))
				{
					FString msg = FString::Printf(TEXT("Effect Applied!: %s"),*tag.GetTagName().ToString());
					GEngine->AddOnScreenDebugMessage(1,8.0f,FColor::Red,msg);

					const FWidgetRow* row =  GetDataTableRowByTag<FWidgetRow>(DataTable,tag);
					MessageWidgetRowSignature.Broadcast(*row);
				}
			}
		}
		);
}

