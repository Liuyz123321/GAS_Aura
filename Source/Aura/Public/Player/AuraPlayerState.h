// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilityInfo;
class ULevelUpInfo;
class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FXPChangedSignature, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FPointsChangedSignature, int)

UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet();

	FORCEINLINE int32 getLevel() const;

	void SetLevel(int32 InLevel);

	void AddToLevel(int32 InLevel);

	int GetXP() const;

	void SetXP(int32 InXP);

	void AddToXP(int32 InXP);

	void AddToAttributePoints(int InPoints);
	
	void AddToSpellPoints(int InPoints);

	int GetAttributePoints() const;

	int GetSpellPoints() const;

	FXPChangedSignature OnXPChangedDelegate;
	
	FXPChangedSignature OnLevelChangedDelegate;

	FPointsChangedSignature OnAttributePointsChangeDelegate;
	
	FPointsChangedSignature OnSpellPointsChangeDelegate;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UAbilityInfo> AbilityInfo;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	
private:
	UPROPERTY(EditAnywhere, ReplicatedUsing=onRep_Level)
	int32 level = 1;

	UPROPERTY(EditAnywhere, ReplicatedUsing=onRep_XP)
	int32 XP = 0;

	UPROPERTY(EditAnywhere, ReplicatedUsing=onRep_AttributePoints)
	int32  AttributePoints= 0;

	UPROPERTY(EditAnywhere, ReplicatedUsing=onRep_SpellPoints)
	int32 SpellPoints = 0;
	
	UFUNCTION()
	void onRep_Level(int32 oldLevel);

	UFUNCTION()
	void onRep_XP(int32 oldXP);

	UFUNCTION()
	void onRep_AttributePoints(int32 oldPoints);

	UFUNCTION()
	void onRep_SpellPoints(int32 oldPoints);
};


inline int AAuraPlayerState::GetXP() const
{
	return XP;
}
