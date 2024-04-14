// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/GetMouseData.h"
#include  "Abilities/Tasks/AbilityTask.h"

UGetMouseData* UGetMouseData::CreateMouseData(UGameplayAbility* OwningAbility)
{
	UGetMouseData* MouseData = NewAbilityTask<UGetMouseData>(OwningAbility);
	
	return MouseData;
}

void UGetMouseData::Activate()
{
	FHitResult HitResult;

	if(APawn* Pawn = Cast<APawn>(this->GetAvatarActor()))
		if(APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController()))
			PlayerController->GetHitResultUnderCursor(ECC_Visibility,false,HitResult);

	FVector Data = HitResult.Location;
	ValidData.Broadcast(Data);
}
