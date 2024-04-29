// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.0f;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	 
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


UAttributeSet* AAuraPlayerState::GetAttributeSet()
{
	return AttributeSet;
}

int32 AAuraPlayerState::getLevel() const
{
	return level;
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	level = InLevel;
	OnLevelChangedDelegate.Broadcast(level);
}

void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	level += InLevel;
	OnLevelChangedDelegate.Broadcast(level);
}

void AAuraPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToAttributePoints(int InPoints)
{
	AttributePoints += InPoints;
	
	OnAttributePointsChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToSpellPoints(int InPoints)
{
	SpellPoints += InPoints;
	
	OnSpellPointsChangeDelegate.Broadcast(SpellPoints);
}

int AAuraPlayerState::GetAttributePoints() const
{
	return AttributePoints;
}

int AAuraPlayerState::GetSpellPoints() const
{
	return SpellPoints;
}


void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, level);
	DOREPLIFETIME(AAuraPlayerState, XP);
}

void AAuraPlayerState::onRep_Level(int32 oldLevel)
{
	OnLevelChangedDelegate.Broadcast(level);
}

void AAuraPlayerState::onRep_XP(int32 oldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::onRep_AttributePoints(int32 oldPoints)
{
	OnAttributePointsChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::onRep_SpellPoints(int32 oldPoints)
{
	OnSpellPointsChangeDelegate.Broadcast(SpellPoints);
}
