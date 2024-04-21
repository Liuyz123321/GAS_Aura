// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecutionCal/ExecutionCal_Damage.h"

#include "AttributesTagsInfo.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

UExecutionCal_Damage::UExecutionCal_Damage()
{
	RelevantAttributesToCapture.Add(GetAuraDamage().ArmorDef);
	RelevantAttributesToCapture.Add(GetAuraDamage().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetAuraDamage().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetAuraDamage().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetAuraDamage().CriticalHitDamageDef);
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

	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	float Damage = GameplayEffectSpec.GetSetByCallerMagnitude(GameplayTags.Damage);
	float BlockChance = 0.f;
	float TargetArmor = 0.f;
	float SourceArmorPenetration = 0.f;
	float SourceCriticalHitChance = 0.f;
	float SourceCriticalHitDamage = 0.f;

	bool bBlocked = false, bCriticalHit = false;
	FGameplayEffectContextHandle GameplayEffectContextHandle = GameplayEffectSpec.GetContext();
	
	// EXAMPLE::
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamage().ArmorDef,AggregatorEvaluateParameters,Armor);
	// Armor = FMath::Max(0,Armor);
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamage().BlockChanceDef,AggregatorEvaluateParameters,BlockChance);
	BlockChance = FMath::Max<float>(0.f,BlockChance);

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamage().ArmorDef,AggregatorEvaluateParameters,TargetArmor);
	TargetArmor = FMath::Max<float>(0.f,TargetArmor);

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamage().ArmorPenetrationDef,AggregatorEvaluateParameters,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f,SourceArmorPenetration);

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamage().CriticalHitChanceDef,AggregatorEvaluateParameters,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f,SourceCriticalHitChance);

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamage().CriticalHitDamageDef,AggregatorEvaluateParameters,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f,SourceCriticalHitDamage);
	
	if(FMath::RandRange(0,100) < BlockChance)
	{
		Damage  = 0;
		UE_LOG(LogTemp,Warning,TEXT("Damage Blocked!"));
		bBlocked = true;
	}
	UAuraAbilitySystemLibrary::SetIsBlocked(bBlocked,GameplayEffectContextHandle);

	float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * 0.25) / 100.f;   //0.25 表示 4 点 ArmorPenetration 能减 1% 防。
	Damage *= (100 - EffectiveArmor * 0.333) / 100.f;                                     //0.33 表示 3 点有效防御力（减防后的防御力） 能免 1% 的伤。

	if(FMath::RandRange(0,100) < SourceCriticalHitChance)
	{
		Damage *= (2.f + (SourceCriticalHitDamage)/100.f);
		bCriticalHit = true;
	}
	UAuraAbilitySystemLibrary::SetIsCriticalHit(bCriticalHit,GameplayEffectContextHandle);
	
	FGameplayModifierEvaluatedData ModifierEvaluatedData(UAuraAttributeSet::GetDamageAttribute(),EGameplayModOp::Override,Damage);
	OutExecutionOutput.AddOutputModifier(ModifierEvaluatedData);
}
