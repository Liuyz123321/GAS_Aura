// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"



AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AAuraEffectActor::OnOverlap(AActor* tarActor)
{
	if(InstantApplyPolicy == EApplyEffectPolicy::OnOverlapApply)
	{
		ApplyEffectToTar(tarActor, InstantGameplayEffectClass);
	}

	if(DurationApplyPolicy == EApplyEffectPolicy::OnOverlapApply)
	{
		ApplyEffectToTar(tarActor, DurationGameplayEffectClass);
	}

	if(InfiniteApplyPolicy == EApplyEffectPolicy::OnOverlapApply)
	{
		ApplyEffectToTar(tarActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::EndOverlap(AActor* tarActor)
{
	if(InstantApplyPolicy == EApplyEffectPolicy::EndOverlapApply)
	{
		ApplyEffectToTar(tarActor, InstantGameplayEffectClass);
	}
	if(DurationApplyPolicy == EApplyEffectPolicy::EndOverlapApply)
	{
		ApplyEffectToTar(tarActor, DurationGameplayEffectClass);
	}

	if(InfiniteApplyPolicy == EApplyEffectPolicy::EndOverlapApply)
	{
		ApplyEffectToTar(tarActor, InfiniteGameplayEffectClass);
	}
	if(InfiniteRemovalPolicy == ERemovalEffectPolicy::EndOverlapRemove)
	{
		IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(tarActor);
		TArray<IAbilitySystemInterface*> HandlesToRemove;
		if(AbilitySystemInterface)
		{
			UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
			for(auto elemPair:EffectHandleMap)
			{
				if(elemPair.Key == AbilitySystemInterface)
				{
					const FActiveGameplayEffectHandle GameplayEffectHandle = elemPair.Value;

					UE_LOG(LogTemp,Warning,TEXT("%s"),*GameplayEffectHandle.ToString());
					bool res = AbilitySystemComponent->RemoveActiveGameplayEffect(GameplayEffectHandle);
					HandlesToRemove.Add(elemPair.Key);
				}
			}
			for(IAbilitySystemInterface* HandleElemToRemove:HandlesToRemove)
			{
				EffectHandleMap.FindAndRemoveChecked(HandleElemToRemove);
			}
		}
	}
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTar(AActor* tarActor, TSubclassOf<UGameplayEffect> EffectClass)
{
	if(!bCanApplyToEnemy && tarActor->ActorHasTag(FName("Enemy"))) return;
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(tarActor);
	if(AbilitySystemInterface)
	{
		UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
		FGameplayEffectContextHandle GameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		GameplayEffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle GameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass,1.0f,GameplayEffectContextHandle);
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle =  AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());

		if(InfiniteRemovalPolicy == ERemovalEffectPolicy::EndOverlapRemove)
		{
			check(AbilitySystemInterface);
			EffectHandleMap.Add(AbilitySystemInterface,ActiveGameplayEffectHandle);
		}

		if(GameplayEffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy != EGameplayEffectDurationType::Infinite) Destroy();
	}
}


