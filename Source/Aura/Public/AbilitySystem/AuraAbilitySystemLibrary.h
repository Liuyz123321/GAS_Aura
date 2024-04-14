// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UMenuWidgetController;
/**
 * 
 */
UCLASS(BlueprintType)
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintPure, Category= "AbilitySystem|WidgetController")
	static UMenuWidgetController* GetAttributeMenuWidgetController(UObject* WorldContext);
};
