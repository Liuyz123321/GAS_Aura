// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AuraAtrributesDataAsset.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "MenuWidgetController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributesInfoSignature,FAuraAttributesInfo,Info);


UCLASS(BlueprintType,Blueprintable)
class AURA_API UMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;
	
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FAttributesInfoSignature AttributesInfo;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAuraAtrributesDataAsset> AuraAttributeDA;
};
