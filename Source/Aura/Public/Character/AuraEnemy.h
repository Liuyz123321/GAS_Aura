// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;
class UBehaviorTree;
struct FGameplayTag;
class UWidgetComponent;
/**
 * 
 */

UENUM()
enum class EEnemyType
{
	Ranger,
	Warrior,
	Elementalist
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthBarChangedSignature,float, NewData);

UCLASS()
class AURA_API AAuraEnemy : public AACharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	virtual void PossessedBy(AController* NewController) override;

	/** begin EnemyInterface*/
	virtual void Highlight() override;
	virtual void unHighlight() override;
	/** end EnemyInterface*/

	virtual int32 getLevel() override;

	void OnTagAdd(const FGameplayTag GameplayTag,int32 NewCount);

	virtual void Die() override;

	bool bHitReact = false;
	
	UPROPERTY(BlueprintReadOnly)
	float BaseWalkSpeed = 250.f;

	
protected:
	virtual void BeginPlay() override;

	virtual void Dissolve() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Default");
	int32 level = 1;

	UPROPERTY(EditAnywhere,Category="AI")
	TObjectPtr<UBehaviorTree>  BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> EnemyAIController;

	UPROPERTY(BlueprintAssignable)
	FOnHealthBarChangedSignature ChangeEnemyProgressBar;

	UPROPERTY(EditAnywhere)
	EEnemyType EnemyType;
private:
	virtual void InitASinfo() override;
};
