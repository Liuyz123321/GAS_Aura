// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAtrributesDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAuraAttributesInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeVal = 0.f;
};
UCLASS()
class AURA_API UAuraAtrributesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	 FAuraAttributesInfo FindAttributeByTag(const FGameplayTag& AttributeTag, bool blogNotFound = false);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FAuraAttributesInfo> AttributesInformation;
};
