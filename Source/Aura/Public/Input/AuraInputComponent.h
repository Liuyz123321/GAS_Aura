// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename  HeldFuncType>
	void BindInputActions(UAuraInputConfig* AuraInputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindInputActions(UAuraInputConfig* AuraInputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(AuraInputConfig);

	for(const FAuraInputAction& info:AuraInputConfig->AuraInputActions)
	{
		if(ReleasedFunc)
		{
			BindAction(info.InputAction,ETriggerEvent::Completed,Object,ReleasedFunc,info.GameplayTag);
		}
		if(PressedFunc)
		{
			BindAction(info.InputAction,ETriggerEvent::Started,Object,PressedFunc,info.GameplayTag);
		}
		if(HeldFunc)
		{
			BindAction(info.InputAction,ETriggerEvent::Triggered,Object,HeldFunc,info.GameplayTag);
		}
	}
}
