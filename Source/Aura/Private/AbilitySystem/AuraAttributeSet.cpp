// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AttributesTagsInfo.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"


UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMana(50.f);

	const FAuraGameplayTags AuraGameplayTags= FAuraGameplayTags::Get();

	AttributesToFunc.Add(AuraGameplayTags.Attribute_Primary_Strength,GetStrengthAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Primary_Intelligence,GetInteligenceAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Primary_Resilience,GetResilianceAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Primary_Vigor,GetVigorAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Secondary_Armor,GetArmorAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Secondary_ArmorPenetration,GetArmorPenetrationAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Secondary_CriticalHitResistance,GetCriticalResistanceAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Secondary_BlockChance,GetBlockChanceAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Secondary_HealthRegeneration,GetHealthRegenerationAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Secondary_ManaRegeneration,GetManaRegenerationAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Secondary_MaxHealth,GetMaxHealthAttribute);
	AttributesToFunc.Add(AuraGameplayTags.Attribute_Secondary_MaxMana,GetMaxManaAttribute);
}

// 当你希望在网络游戏中同步某个属性时，你需要在对应的属性集合类中调用 GetLifetimeReplicatedProps 函数来声明该属性的同步规则。
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Inteligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Resiliance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Armor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,ArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,BlockChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,HealthRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,ManaRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp,Warning,TEXT("health:%f"),GetHealth());
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}
	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties props;
	SetProperties(Data,props);

	
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float curHealth = GetHealth();
		SetHealth(FMath::Clamp(curHealth,0.f,GetMaxHealth()));
		
	}

	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
		UE_LOG(LogTemp,Warning,TEXT("Changed Health on %s, Health: %f "),*props.TargetCharacter->GetName(),GetHealth())
	}

	
	// Here to use FEffectProperties
	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float InComingDamage = GetDamage();
		SetDamage(0);
		if(InComingDamage >= 0)
		{
			const float NewHealth = GetHealth() - InComingDamage;
			UE_LOG(LogTemp,Warning,TEXT("[%s]'s Health becomes %f"),*props.TargetCharacter->GetName(),NewHealth)
			SetHealth(FMath::Clamp(NewHealth,0,GetMaxHealth()));

			if(NewHealth >= 0)
			{
				FGameplayTagContainer GameplayTags;
				GameplayTags.AddTag(FAuraGameplayTags::Get().Effect_HitReact);
				bool bActivated = props.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(GameplayTags);
			}

			else
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(props.TargetAvatarActor);
				CombatInterface->Die();
			}

			if(props.SourceCharacter != props.TargetCharacter)
			{
				FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(props.EffectContextHandle.Get());
				bool IsBlock = AuraGameplayEffectContext->GetIsBlock();
				bool IsCritic = AuraGameplayEffectContext->GetIsCriticalHit();
				
				if(AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(props.SourceController))
				{
					PlayerController->SpawnDamageText(InComingDamage,props.TargetCharacter,IsCritic,IsBlock);
					return;
				}  
				if(AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(props.TargetController))
				{
					PlayerController->SpawnDamageText(InComingDamage,props.TargetCharacter,IsCritic,IsBlock);
				}
			}
		}
	}


	
}


void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Strength,OldStrength);
}

void UAuraAttributeSet::OnRep_Inteligence(const FGameplayAttributeData& OldInteligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Inteligence,OldInteligence);
}

void UAuraAttributeSet::OnRep_Resiliance(const FGameplayAttributeData& OldResiliance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Resiliance,OldResiliance);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Vigor,OldVigor);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	//这个宏会触发一个事件，该事件会通知所有监听该属性变化的蓝图对象，以便它们能够响应属性的变化。
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxHealth,OldMaxHealth);                                                          
} 

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldMana);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalResistance(const FGameplayAttributeData& OldCriticalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldCriticalResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxMana,OldMaxMana);
}

void UAuraAttributeSet::SetProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const
{
	FGameplayEffectContextHandle GameplayEffectContextHandle;
	if(Data.EffectSpec.GetEffectContext().IsValid())
	{
		GameplayEffectContextHandle = Data.EffectSpec.GetEffectContext();
	}
	EffectProperties.EffectContextHandle = GameplayEffectContextHandle;

	/* 处理target */
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid() && Data.Target.AbilityActorInfo->AbilitySystemComponent.IsValid())
	{
		AActor* TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProperties.TargetAvatarActor = TargetAvatarActor;
		
		if(Cast<APawn>(TargetAvatarActor))
			EffectProperties.TargetController = Cast<APawn>(TargetAvatarActor)->GetController();

		if(Cast<ACharacter>(TargetAvatarActor))
			EffectProperties.TargetCharacter = Cast<ACharacter>(TargetAvatarActor);

		EffectProperties.TargetAbilitySystemComponent = Data.Target.AbilityActorInfo->AbilitySystemComponent.Get();
	}

	/* 处理source */
	if(GameplayEffectContextHandle.IsValid() && GameplayEffectContextHandle.GetOriginalInstigator())
	{
		UAbilitySystemComponent* SourceASC = GameplayEffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
		EffectProperties.SourceAbilitySystemComponent = SourceASC;

		AActor* SourceActor = SourceASC->GetAvatarActor();
		EffectProperties.SourceAvatarActor = SourceActor;
		
		if(Cast<APawn>(SourceActor))
			EffectProperties.SourceController = Cast<APawn>(SourceActor)->GetController();

		if(Cast<ACharacter>(SourceActor))
			EffectProperties.SourceCharacter = Cast<ACharacter>(SourceActor);
	}
}
