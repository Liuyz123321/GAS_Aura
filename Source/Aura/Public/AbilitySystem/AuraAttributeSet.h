// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

template<class T>
using StaticFunc = typename TBaseStaticDelegateInstance<T,FDefaultDelegateUserPolicy>::FFuncPtr;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}
	
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;
	
	UPROPERTY()
	UAbilitySystemComponent* TargetAbilitySystemComponent = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;
 
	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
	FGameplayEffectContextHandle EffectContextHandle;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, StaticFunc<FGameplayAttribute()>> AttributesToFunc;

	//Primary Attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="Primary Attribute")
	FGameplayAttributeData Strength;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Inteligence, Category="Primary Attribute")
	FGameplayAttributeData Inteligence;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resiliance, Category="Primary Attribute")
	FGameplayAttributeData Resiliance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vigor, Category="Primary Attribute")
	FGameplayAttributeData Vigor;
	
	//Vital Attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Vital Attribute")
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vital Attribute")
	FGameplayAttributeData Mana;

	//Secondary Attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Secondary Attribute")
	FGameplayAttributeData Armor;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ArmorPenetration, Category="Secondary Attribute")
	FGameplayAttributeData ArmorPenetration;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BlockChance, Category="Secondary Attribute")
	FGameplayAttributeData BlockChance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitChance, Category="Secondary Attribute")
	FGameplayAttributeData CriticalHitChance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitDamage, Category="Secondary Attribute")
	FGameplayAttributeData CriticalHitDamage;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalResistance, Category="Secondary Attribute")
	FGameplayAttributeData CriticalResistance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HealthRegeneration, Category="Secondary Attribute")
	FGameplayAttributeData HealthRegeneration;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ManaRegeneration, Category="Secondary Attribute")
	FGameplayAttributeData ManaRegeneration;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Secondary Attribute")
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Secondary Attribute")
	FGameplayAttributeData MaxMana;

	//Meta Attributes
	UPROPERTY(BlueprintReadOnly, Category="Meta Attribute")
	FGameplayAttributeData Damage;
	UPROPERTY(BlueprintReadOnly, Category="Meta Attribute")
	FGameplayAttributeData InComingXP;
	
	
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Health)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Mana)
	
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Strength)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Inteligence)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Resiliance)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Vigor)
	
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Armor)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ArmorPenetration)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,BlockChance)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitChance)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitDamage)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalResistance)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,HealthRegeneration)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ManaRegeneration)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxHealth)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxMana)

	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Damage)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,InComingXP)

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Inteligence(const FGameplayAttributeData& OldInteligence) const;
	UFUNCTION()
	void OnRep_Resiliance(const FGameplayAttributeData& OldResiliance) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	UFUNCTION()
	void OnRep_CriticalResistance(const FGameplayAttributeData& OldCriticalResistance) const;
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

private:
	void SetProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const;
	
	void SendGainXPEvent(const FEffectProperties& EffectProperties);
};
