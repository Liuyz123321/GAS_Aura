// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

/**
 * 
 */

class UDamageTextComponent;
class AACharacter;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
struct FAuraInputAction;
class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	virtual void CursorTrace();

	void SpawnDamageText(float InComingDamage, ACharacter* TargetCharacter);
protected:
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageTextComponent> DamageTextCompClass;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;
	
	void Move(const FInputActionValue& InputActionValue);

	void AbilityInputTagPressed(FGameplayTag Tag);
	void AbilityInputTagReleased(FGameplayTag Tag);
	void AbilityInputTagHeld(FGameplayTag Tag);

	bool bShiftIsPressed = false;
	void ShiftInputPressed(){bShiftIsPressed = true;}
	void ShiftInputReleased(){bShiftIsPressed = false;}

	IEnemyInterface *lastCursor;
	IEnemyInterface *thisCursor;

	UPROPERTY(EditDefaultsOnly)
	UAuraInputConfig* AuraInputConfig;

	UPROPERTY()
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = nullptr;

	float FollowTime = 0.f;
	FVector CacheMoveLocation = FVector::ZeroVector;
	bool IsAutoRunning = false;
	float ShortPressThreshold = 0.5f;

	bool IsTargetingSth = false;

	UPROPERTY(EditAnywhere)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
	UAuraAbilitySystemComponent* GetASC();
};
