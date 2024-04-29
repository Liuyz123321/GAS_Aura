// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FAbilityDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag CoolDownTag = FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag StateTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackGroundMaterial = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuraGameplayAbility> Ability;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int RequireLevel = 1;
};

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FAbilityDefaultInfo> AbilityDefaultInfos;

	FAbilityDefaultInfo FindInfoByTag(const FGameplayTag& Tag);
};
