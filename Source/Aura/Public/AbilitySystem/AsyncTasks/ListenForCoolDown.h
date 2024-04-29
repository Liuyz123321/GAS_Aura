// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "ListenForCoolDown.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoolDownChangedSigature, float, RemaingSeconds);
/**
 * 
 */
UCLASS(BlueprintType,meta=(ExposedAsyncProxy = "AsyncTask"))
class AURA_API UListenForCoolDown : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCoolDownChangedSigature OnCoolDownStart;
	
	UPROPERTY(BlueprintAssignable)
	FCoolDownChangedSigature OnCoolDownEnd;

	UFUNCTION(BlueprintCallable,meta=(BlueprintInternalUseOnly = "true"))
	static UListenForCoolDown* WaitForCoolDownChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag InCoolDownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	void OnCoolDownChange(const FGameplayTag InCoolDownTag, int NewCount);

	void OnEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

private:

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	FGameplayTag CoolDownTag;
};
