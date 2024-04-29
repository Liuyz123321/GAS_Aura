// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/Data/LevelUpInfo.h"


AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,400,0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	LevelUpNiagara = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagara");
	LevelUpNiagara->SetupAttachment(GetRootComponent());
	LevelUpNiagara->bAutoActivate = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false; 
	bUseControllerRotationYaw = false;
}

int32 AAuraCharacter::getLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->getLevel();
}

void AAuraCharacter::AddXp_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddToXP(InXP);
}

int AAuraCharacter::GetLevelForXP_Implementation(int32 XP)
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	check(LevelUpInfo);

	return LevelUpInfo->FindLevelForXP(XP);
}

void AAuraCharacter::LevelUp_Implementation(int32 InLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
	check(AbilitySystemComponent);
	check(AuraPlayerState);

	check(LevelUpNiagara);
	LevelUpNiagara->Activate(true);
	
	AuraPlayerState->SetLevel(InLevel);
	AuraASC->UpdateAbilityStatuses(InLevel);
}

void AAuraCharacter::AddAttributeAward_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddSpellAward_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

int AAuraCharacter::GetAttributeAwardForLevel_Implementation(int32 Level)
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	check(LevelUpInfo);

	return LevelUpInfo->LevelUpInfos[Level].AttributePointAward;
}

int AAuraCharacter::GetSpellAwardForLevel_Implementation(int32 Level)
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	check(LevelUpInfo);

	return LevelUpInfo->LevelUpInfos[Level].SpellPointAward;
}

int AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->getLevel();
}

int AAuraCharacter::GetXP_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetXP();
}

int AAuraCharacter::GetAttributePoints_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetAttributePoints();
}

int AAuraCharacter::GetSpellPoints_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetSpellPoints();
}

bool AAuraCharacter::IsNoAttributePoints_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetAttributePoints() == 0;
}

// FLevelUpDetailedInfo AAuraCharacter::GetLevelUpInfoForXP_Implementation(int32 InXP)
// {
// 	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
// 	check(AuraPlayerState);
// 	return AuraPlayerState->LevelUpInfo->FindLevelUpInfoForXP(InXP);
// }

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitASinfo();
	AddAbilities();
}


void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitASinfo();
}

void AAuraCharacter::InitASinfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->OnASinfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController,AuraPlayerState,AttributeSet,AbilitySystemComponent);
		}
	}

	InitDefaultAttributes();
}
