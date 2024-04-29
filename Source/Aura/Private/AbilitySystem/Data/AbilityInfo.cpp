// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

FAbilityDefaultInfo UAbilityInfo::FindInfoByTag(const FGameplayTag& Tag)
{
	for(FAbilityDefaultInfo Elem:AbilityDefaultInfos)
	{
		if(Elem.AbilityTag == Tag)
		{
			return Elem;
		}
	}

	return  FAbilityDefaultInfo();
}
