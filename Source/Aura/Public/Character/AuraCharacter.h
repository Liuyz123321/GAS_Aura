// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AACharacter
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual int32 getLevel() override;

protected:
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

private:
	virtual void InitASinfo() override;
};
