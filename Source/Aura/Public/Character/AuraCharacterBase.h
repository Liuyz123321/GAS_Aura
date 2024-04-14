// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UDamageTextComponent;
class UAuraGameplayAbility;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;

UCLASS(Abstract)
class AURA_API AACharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AACharacter();

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> weapon;

	UPROPERTY(EditAnywhere)
	FName WeaponTipSocketName;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAtrribueSet();

	virtual FVector GetWeaponTipSocketLocation() override;

	virtual UAnimMontage* GetAnimMontage_Implementation() override;

	virtual void Die() override;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void applyEffect(TSubclassOf<UGameplayEffect> effect, float level) const;
	
	void InitDefaultAttributes();

	void AddAbilities();

	virtual void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void MeshDissolveTimeLine(UMaterialInstanceDynamic* MaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponDissolveTimeLine(UMaterialInstanceDynamic* MaterialInstance);

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attribute")
	TSubclassOf<UGameplayEffect> SetPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attribute")
	TSubclassOf<UGameplayEffect> SetSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attribute")
	TSubclassOf<UGameplayEffect> SetVitalAttributes;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> MeshDissolveMaterialInstance;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;
	
private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	virtual void InitASinfo();
	
};
