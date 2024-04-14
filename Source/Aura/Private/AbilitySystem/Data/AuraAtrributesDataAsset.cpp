// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AuraAtrributesDataAsset.h"

FAuraAttributesInfo UAuraAtrributesDataAsset::FindAttributeByTag(const FGameplayTag& AttributeTag, bool blogNotFound)
{
	for(auto attributeInfo: AttributesInformation)
	{
		if(attributeInfo.AttributeTag == AttributeTag)
		{
			return attributeInfo;
		}
	}

	if(blogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can not find Info for AttributeTag [%s] on AttributesInformation [%s]"), *AttributeTag.ToString(),*GetNameSafe(this));
	}

	return FAuraAttributesInfo(); 
}
