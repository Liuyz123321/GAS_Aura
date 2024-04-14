// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GetMouseData.generated.h"

class UGameplayAbility;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetMouseDataDelegate, FVector ,MouseData);

UCLASS()
class AURA_API UGetMouseData : public UAbilityTask
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "TargetUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = true))
	static UGetMouseData* CreateMouseData(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FTargetMouseDataDelegate ValidData;

private:
	virtual void Activate() override;
};
