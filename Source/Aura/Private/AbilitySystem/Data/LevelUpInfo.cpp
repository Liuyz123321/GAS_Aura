// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelUpInfo.h"

FLevelUpDetailedInfo ULevelUpInfo::FindLevelUpInfoForXP(const int32 INXP) const
{
	int MaxLevel = LevelUpInfos.Num(), i = 1;

	for(; i < MaxLevel; i++)
	{
		if(INXP < LevelUpInfos[i].XPDemand)
		{
			return LevelUpInfos[i];
		}
	}

	return LevelUpInfos[i];
}

int ULevelUpInfo::FindLevelForXP(const int32 INXP) const
{
	int MaxLevel = LevelUpInfos.Num(), i = 1;

	for(; i < MaxLevel; i++)
	{
		if(INXP < LevelUpInfos[i].XPDemand)
		{
			return i;
		}
	}

	return i;
}

