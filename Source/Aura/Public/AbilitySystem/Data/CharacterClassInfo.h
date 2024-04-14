// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass
{
	Elementalist,
	Ranger,
	Warrior
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};
/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> VitalAttributes;

	FCharacterClassDefaultInfo GetCharacterClassDefaultInfo(ECharacterClass CharacterClass);
};
