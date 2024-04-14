// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraWidgetController;
class UOverlayWidgetController;
class UMenuWidgetController;
struct FWidgetControllerParams;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> AuraUserWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	
	UMenuWidgetController* GetMenuWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	void InitOverlay(APlayerController *PC, APlayerState *PS, UAttributeSet *AS, UAbilitySystemComponent *ASC);

protected:
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> AuraUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY()
	TObjectPtr<UMenuWidgetController> MenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraWidgetController> AuraOverlayWidgetControllerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraWidgetController> AuraMenuWidgetControllerClass;
};
