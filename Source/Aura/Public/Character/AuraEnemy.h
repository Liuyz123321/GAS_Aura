// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AACharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	/** begin EnemyInterface*/
	virtual void Highlight() override;
	virtual void unHighlight() override;
	/** end EnemyInterface*/

protected:
	virtual void BeginPlay() override;
	
};
