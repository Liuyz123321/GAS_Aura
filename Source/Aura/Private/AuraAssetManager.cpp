// Fill out your copyright notice in the Description page of Project Settings.
#include "AuraAssetManager.h"
#include "AttributesTagsInfo.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	return *Cast<UAuraAssetManager>(GEngine->AssetManager);
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitNativeGameplayTags();
}
