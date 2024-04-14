// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributesTagsInfo.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitNativeGameplayTags()
{
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor"),FString(""));
	GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ArmorPenetration"),FString(""));
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.BlockChance"),FString(""));
	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitChance"),FString(""));
	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitDamage"),FString(""));
	GameplayTags.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitResistance"),FString(""));
	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.HealthRegeneration"),FString(""));
	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ManaRegeneration"),FString(""));
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxHealth"),FString(""));
	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxMana"),FString(""));
	
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Strength"),FString(""));
	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Intelligence"),FString(""));
	GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Resilience"),FString(""));
	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Vigor"),FString(""));
	
	GameplayTags.Input_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.1"),FString(""));
	GameplayTags.Input_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.2"),FString(""));
	GameplayTags.Input_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.3"),FString(""));
	GameplayTags.Input_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.4"),FString("")); 
	GameplayTags.Input_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.LMB"),FString(""));
	GameplayTags.Input_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.RMB"),FString(""));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),FString(""));
	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.HitReact"),FString(""));
}
