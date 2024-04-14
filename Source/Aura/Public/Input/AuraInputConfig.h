// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameplayTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction = nullptr;
};
/**
 * 
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UInputAction* GetInputActionByTag(FGameplayTag GameplayTag, bool blogNotFound) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AuraInputActions;
};
