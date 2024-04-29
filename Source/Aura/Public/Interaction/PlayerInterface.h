// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void AddXp(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp(int32 InLevel);

	UFUNCTION(BlueprintNativeEvent)
	int GetLevelForXP(int32 XP);
	
	UFUNCTION(BlueprintNativeEvent)
	int GetAttributeAwardForLevel(int32 Level);
	
	UFUNCTION(BlueprintNativeEvent)
	int GetSpellAwardForLevel(int32 Level);

	UFUNCTION(BlueprintNativeEvent)
	void AddAttributeAward(int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddSpellAward(int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent)
	int GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent)
	int GetXP();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	int GetAttributePoints();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	int GetSpellPoints();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	bool IsNoAttributePoints();
	
	//UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	// FLevelUpDetailedInfo GetLevelUpInfoForXP(int32 InXP);
};
