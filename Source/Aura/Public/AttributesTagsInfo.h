// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */

struct FAuraGameplayTags
{
 
public: 
 static const FAuraGameplayTags& Get() {return GameplayTags;}
 static void InitNativeGameplayTags();

 FGameplayTag Attribute_Primary_Strength;
 FGameplayTag Attribute_Primary_Intelligence;
 FGameplayTag Attribute_Primary_Resilience;
 FGameplayTag Attribute_Primary_Vigor;
 
 FGameplayTag Attribute_Secondary_Armor;
 FGameplayTag Attribute_Secondary_ArmorPenetration;
 FGameplayTag Attribute_Secondary_BlockChance;
 FGameplayTag Attribute_Secondary_CriticalHitChance;
 FGameplayTag Attribute_Secondary_CriticalHitDamage;
 FGameplayTag Attribute_Secondary_CriticalHitResistance;
 FGameplayTag Attribute_Secondary_HealthRegeneration;
 FGameplayTag Attribute_Secondary_ManaRegeneration;
 FGameplayTag Attribute_Secondary_MaxHealth;
 FGameplayTag Attribute_Secondary_MaxMana;

 FGameplayTag Input_1;
 FGameplayTag Input_2;
 FGameplayTag Input_3;
 FGameplayTag Input_4;
 FGameplayTag Input_LMB;
 FGameplayTag Input_RMB;

 FGameplayTag Damage;
 FGameplayTag Effect_HitReact;
 
protected:
 
private:
 static FAuraGameplayTags GameplayTags;
};
