// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "ExecutionCal_Damage.generated.h"

/**
 * 
 */
struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false)
	}
};

static const AuraDamageStatics& GetAuraDamage()
{
	static AuraDamageStatics DamageStatics;
	return DamageStatics;
}
UCLASS()
class AURA_API UExecutionCal_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	
	UExecutionCal_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
