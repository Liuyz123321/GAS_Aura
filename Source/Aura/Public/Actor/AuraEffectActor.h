// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class IAbilitySystemInterface;

UENUM(BlueprintType)
enum class EApplyEffectPolicy
{
	DoNotApply,
	OnOverlapApply,
	EndOverlapApply
};

UENUM(BlueprintType)
enum class ERemovalEffectPolicy
{
	DoNotRemove,
	EndOverlapRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();  

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* tarActor);

	UFUNCTION(BlueprintCallable)
	void EndOverlap(AActor* tarActor);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Enemy")
	bool bCanApplyToEnemy = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	EApplyEffectPolicy InstantApplyPolicy = EApplyEffectPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	EApplyEffectPolicy DurationApplyPolicy = EApplyEffectPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	EApplyEffectPolicy InfiniteApplyPolicy = EApplyEffectPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	ERemovalEffectPolicy InfiniteRemovalPolicy = ERemovalEffectPolicy::EndOverlapRemove;

	TMap<IAbilitySystemInterface*, FActiveGameplayEffectHandle> EffectHandleMap;
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTar(AActor* tarActor, TSubclassOf<UGameplayEffect> EffectClass);
	
};
