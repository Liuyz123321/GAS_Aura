// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

UInputAction* UAuraInputConfig::GetInputActionByTag(FGameplayTag GameplayTag, bool blogNotFound) const
{
	for(auto info: AuraInputActions)
	{
		if(info.InputAction && info.GameplayTag == GameplayTag)
			return info.InputAction;
	}

	if(blogNotFound)
	{
		UE_LOG(LogTemp,Error,TEXT("Can not find by tag:[%s] on InputConfig:[%s]"),*GameplayTag.ToString(),*GetNameSafe(this));
	}
	
	return nullptr;
}
