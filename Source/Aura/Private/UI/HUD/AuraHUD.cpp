    // Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/MenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widgets/AuraUserWidget.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,AuraOverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
	}
	return OverlayWidgetController;
}

UMenuWidgetController* AAuraHUD::GetMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if(MenuWidgetController == nullptr)
	{
		MenuWidgetController = NewObject<UMenuWidgetController>(this,AuraMenuWidgetControllerClass);
		MenuWidgetController->SetWidgetControllerParams(WCParams);
	}
	return MenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAttributeSet* AS, UAbilitySystemComponent* ASC)
{
	checkf(AuraUserWidgetClass,TEXT("Set AuraUserWidgetClass Before InitOverlay!"));
	checkf(AuraOverlayWidgetControllerClass,TEXT("Set AuraOverlayWidgetControllerClass Before InitOverlay!"));
	UAuraUserWidget* widget = CreateWidget<UAuraUserWidget>(GetWorld(), AuraUserWidgetClass);
	AuraUserWidget = widget;

	FWidgetControllerParams WCParams(PC,PS,AS,ASC);
	UOverlayWidgetController* MOverlayWidgetController = GetOverlayWidgetController(WCParams);
	widget->SetWidgetController(MOverlayWidgetController);

	MOverlayWidgetController->BroadcastInitialValues();
	
	widget->AddToViewport();
}


