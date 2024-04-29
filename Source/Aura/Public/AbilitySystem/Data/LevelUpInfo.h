// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpDetailedInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 XPDemand;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 AttributePointAward;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 SpellPointAward;
};
/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FLevelUpDetailedInfo> LevelUpInfos;

	UFUNCTION(BlueprintCallable)
	FLevelUpDetailedInfo FindLevelUpInfoForXP(const int32 INXP) const;

	UFUNCTION(BlueprintCallable)
	int FindLevelForXP(const int32 INXP) const;
};
