// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAbilityInfo;

USTRUCT(BlueprintType)
struct FWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag GameplayTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText message = FText();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature,float,OnHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature,float,OnMaxHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature,float,OnManaChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManathChangedSignature,float,OnMaxManaChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature,float,NewVal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChangedSignature,int,NewVal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,FWidgetRow,row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature,FAbilityDefaultInfo,AbilityDefaultInfo);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMaxManathChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable)
	FMessageWidgetRowSignature MessageWidgetRowSignature;

	UPROPERTY(BlueprintAssignable)
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnXPBarChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnLevelChangedSignature OnLevelChangedDelegate;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* DataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& tag);

	void OnAbilitiesGiven();

	void OnXPChanged(int32 InXP);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& tag)
{
	return DataTable->FindRow<T>(tag.GetTagName(),FString(""));
}
