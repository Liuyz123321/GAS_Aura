// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,FWidgetRow,row);

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

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* DataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& tag);
	
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& tag)
{
	return DataTable->FindRow<T>(tag.GetTagName(),FString(""));
}
