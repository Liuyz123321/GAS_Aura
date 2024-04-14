// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams(){}

	FWidgetControllerParams(APlayerController *PC, APlayerState *PS, UAttributeSet *AS, UAbilitySystemComponent *ASC):
	PlayerController(PC),PlayerState(PS),AttributeSet(AS),AbilitySystemComponent(ASC)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerController* PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerState* PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributeSet* AttributeSet = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;
};

UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependencies();
protected:
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
