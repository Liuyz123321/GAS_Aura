// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FAggregatorEvaluateParameters AggregatorEvaluateParameters;
	float vigor = 0;
	GetCapturedAttributeMagnitude(VigorDef,Spec,AggregatorEvaluateParameters,vigor);
	const int32 level = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject())->getLevel();

	return 80 + 2.5*vigor + 10*level;
}
