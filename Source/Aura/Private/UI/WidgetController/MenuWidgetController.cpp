// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MenuWidgetController.h"

#include "AttributesTagsInfo.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet *as = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AuraAttributeDA);
	
	for(auto info:as->AttributesToFunc)
	{
		FGameplayTag GameplayTag = info.Key;
		
		FAuraAttributesInfo AuraAttributesInfo = AuraAttributeDA->FindAttributeByTag(GameplayTag,true);
		AuraAttributesInfo.AttributeVal = info.Value().GetNumericValue(as);
		AttributesInfo.Broadcast(AuraAttributesInfo);
	}
	
	BindCallbacksToDependencies();
}

void UMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet *as = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AuraAttributeDA);
	check(AbilitySystemComponent);

	for(auto info:as->AttributesToFunc)
	{
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(info.Value()).AddLambda(
			[this,info,as](const FOnAttributeChangeData&)
			{
				const FGameplayTag GameplayTag = info.Key;
				FAuraAttributesInfo AuraAttributesInfo = AuraAttributeDA->FindAttributeByTag(GameplayTag,true);
				AuraAttributesInfo.AttributeVal = info.Value().GetNumericValue(as);
				AttributesInfo.Broadcast(AuraAttributesInfo);
			}
			);
	}
}
