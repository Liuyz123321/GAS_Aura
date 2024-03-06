

#include "Character\AuraCharacterBase.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


AACharacter::AACharacter()
{

	PrimaryActorTick.bCanEverTick = false;

	weapon = CreateDefaultSubobject<USkeletalMeshComponent>("weapon");
	weapon->SetupAttachment(GetMesh(),"WeaponHandSocket");
	weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// Called when the game sta rts  or when spawned
void AACharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

