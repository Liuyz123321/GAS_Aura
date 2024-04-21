// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraAttackAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AttributesTagsInfo.h"

void UAuraAttackAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle =MakeOutgoingGameplayEffectSpec(DamageEffect,1.0f);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpecHandle,FAuraGameplayTags::Get().Damage,DamageNumber);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	
}
