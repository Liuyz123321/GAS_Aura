#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool GetIsCriticalHit() const
	{
		return IsCriticalHit; 
	}

	bool GetIsBlock() const
	{
		return IsBlocked;
	}

	void SetIsCriticalHit(bool IsInCriticalHit)
	{
		IsCriticalHit = IsInCriticalHit;
	}

	void SetIsBlock(bool IsInBlock)
	{
		IsBlocked = IsInBlock;
	}
	
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	
protected:
	
	UPROPERTY()
	bool IsCriticalHit = false;

	UPROPERTY()
	bool IsBlocked = false;
	
private:
	
};
