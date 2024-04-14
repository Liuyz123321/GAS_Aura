// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/MenuWidgetController.h"

UMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(UObject* WorldContext)
{
	if(APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(WorldContext,0)))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttribueSet();

			const FWidgetControllerParams WidgetControllerParams(PC,PS,AS,ASC);
			UMenuWidgetController* MenuWidgetController = AuraHUD->GetMenuWidgetController(WidgetControllerParams);
			
			return MenuWidgetController;
		}
	}

	return nullptr;
}
