// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecutionCal/ExecutionCal_Damage.h"

#include "Engine/SpecularProfile.h"

UExecutionCal_Damage::UExecutionCal_Damage()
{
	RelevantAttributesToCapture.Add(GetAuraDamage().ArmorDef);
}

void UExecutionCal_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                  FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceActor = SourceASC->GetAvatarActor();
	const AActor* TargetActor = TargetASC->GetAvatarActor();

	const FGameplayEffectSpec GameplayEffectSpec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = GameplayEffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = GameplayEffectSpec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters AggregatorEvaluateParameters;
	AggregatorEvaluateParameters.SourceTags = SourceTags;
	AggregatorEvaluateParameters.TargetTags = TargetTags;

	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamage().ArmorDef,AggregatorEvaluateParameters,Armor);
	Armor = FMath::Max(0,Armor);
	
}
