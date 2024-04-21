// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.


USTRUCT(BlueprintType)
struct FMontageTag
{
	GENERATED_BODY()              
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag MontageTag;
};

UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int getLevel();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FVector GetWeaponTipSocketLocation(const FGameplayTag& GameplayTag);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetFacingTarget(const FVector& FacingTargetLocation);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAnimMontage* GetHitReactAnimMontage();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FMontageTag GetMeleeAttackAnimMontageAndTag();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool IsDead();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	TArray<FMontageTag> GetMontageTags();

	UFUNCTION()

	virtual void Die() = 0;
};
