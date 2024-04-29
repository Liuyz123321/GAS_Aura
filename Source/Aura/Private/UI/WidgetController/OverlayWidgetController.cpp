// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

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

	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);

	AuraPlayerState->OnXPChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);
	
	AuraPlayerState->OnLevelChangedDelegate.AddLambda([this](int NewLevel)
	{
		OnLevelChangedDelegate.Broadcast(NewLevel);
	});
	
	
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

	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if(AuraASC->bAbilitiesGiven)
		{
			OnAbilitiesGiven();
		}
		else
		{
			AuraASC->AbilitiesGiven.AddUObject(this,&UOverlayWidgetController::OnAbilitiesGiven);
		}
		
		AuraASC->EffectTags.AddLambda(
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
}

void UOverlayWidgetController::OnAbilitiesGiven()
{
	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		FForEachAbility ForEachAbilityDelegate;

		ForEachAbilityDelegate.BindLambda([this,AuraASC](const FGameplayAbilitySpec& Spec)
		{
			FAbilityDefaultInfo AbilityDefaultInfo = AbilityInfo->FindInfoByTag(AuraASC->FindAbilityTagForSpec(Spec));
			const FGameplayTag InputTag = AuraASC->FindInputTagForSpec(Spec);
			AbilityDefaultInfo.InputTag = InputTag;
			// UE_LOG(LogTemp,Warning,TEXT("%s"),*AbilityDefaultInfo.InputTag.GetTagName().ToString());
			AbilityInfoDelegate.Broadcast(AbilityDefaultInfo);
		});

		AuraASC->ForEachAbility(ForEachAbilityDelegate);
	}
}

void UOverlayWidgetController::OnXPChanged(int32 InXP) 
{
	float XPPercent = 0.f;

	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	checkf(LevelUpInfo,TEXT("level up Info Not Set!"));

	int CurLevel = LevelUpInfo->FindLevelForXP(InXP);
	int BeginXP = LevelUpInfo->LevelUpInfos[CurLevel - 1].XPDemand;
	int EndXP = LevelUpInfo->LevelUpInfos[CurLevel].XPDemand;
	
	XPPercent = static_cast<float>(InXP - BeginXP) / static_cast<float>(EndXP - BeginXP);
	
	OnXPBarChangedDelegate.Broadcast(XPPercent);
}

