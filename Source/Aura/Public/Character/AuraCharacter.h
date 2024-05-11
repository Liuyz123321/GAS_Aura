// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "NiagaraComponent.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AACharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual int32 getLevel() override;
	
	virtual void AddXp_Implementation(int32 InXP) override;

	virtual int GetLevelForXP_Implementation(int32 XP) override;

	virtual void LevelUp_Implementation(int32 InLevel) override;

	virtual void AddAttributeAward_Implementation(int32 InAttributePoints) override;

	virtual void AddSpellAward_Implementation(int32 InSpellPoints) override;

	virtual int GetAttributeAwardForLevel_Implementation(int32 Level) override;

	virtual int GetSpellAwardForLevel_Implementation(int32 Level) override;

	virtual int GetPlayerLevel_Implementation() override;

	virtual int GetXP_Implementation() override;

	virtual int GetAttributePoints_Implementation() override;

	virtual int GetSpellPoints_Implementation() override;

	virtual bool IsNoAttributePoints_Implementation() override;

protected:
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

private:
	virtual void InitASinfo() override;
};
