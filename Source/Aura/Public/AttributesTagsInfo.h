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
 
 FGameplayTag Attribute_Meta_InComingXP;
 
 FGameplayTag Input_1;
 FGameplayTag Input_2;
 FGameplayTag Input_3;
 FGameplayTag Input_4;
 FGameplayTag Input_LMB;
 FGameplayTag Input_RMB;
 FGameplayTag Input_MMB;

 FGameplayTag Damage;
 FGameplayTag Effect_HitReact;

 FGameplayTag Ability_Attack;
 
 FGameplayTag Abilities_Fire_FireBolt;
 FGameplayTag Abilities_Fire_FireBolts;
 FGameplayTag Abilities_Thunder_1;
 FGameplayTag Abilities_None;

 FGameplayTag Abilities_Offensive;
 FGameplayTag Abilities_Passive;

 FGameplayTag Abilities_State_Unlock;
 FGameplayTag Abilities_State_Lock;
 FGameplayTag Abilities_State_Equipped;
 FGameplayTag Abilities_State_Ready;
 
 FGameplayTag CoolDown_Fire_FireBolt;
 
 FGameplayTag Montage_Attack_Weapon;
 FGameplayTag Montage_Attack_LeftHand;
 FGameplayTag Montage_Attack_RightHand;

 
private:
 static FAuraGameplayTags GameplayTags;
};
