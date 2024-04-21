// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ShaderPrintParameters.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);

	/** SetCollisionEnabled是一个函数，用于设置物体或组件的碰撞状态。
	在UE4中，物体或组件可以有不同的碰撞状态，包括以下几种：
	* NoCollision（无碰撞）：物体或组件不参与碰撞检测。
	* QueryOnly（仅查询）：物体或组件可以被其他物体查询碰撞，但不会产生物理响应。
	* PhysicsOnly（仅物理）：物体或组件具有物理行为，会与其他物体发生碰撞并产生物理响应。
	* QueryAndPhysics（查询和物理）：物体或组件既可以被查询碰撞，又会产生物理响应。  **/
	
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::SetSpawnActor(AActor* Actor)
{
	SpawnActor = Actor;
}

AActor* AAuraProjectile::GetSpawnActor() const
{
	return SpawnActor;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AAuraProjectile::OnSphereOverlap);

	AudioComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound,GetRootComponent());
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& FHitResult)
{
	if(OtherActor == SpawnActor) return;
	
 	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	if(IsValid(AudioComponent))
		AudioComponent->Stop();

	if(HasAuthority())
	{
		if(UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
		}
		Destroy();
	}
}


