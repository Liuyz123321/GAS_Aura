// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APawn * OwningPawn = AIOwner->GetPawn();

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player"))?FName("Enemy"):FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn,TargetTag,ActorsWithTag);

	AActor * TargetToFollow = nullptr;
	float DistanceToTarget = TNumericLimits<float>::Max();
	
	for(AActor* Actor:ActorsWithTag)
	{
		const float tmpDist = Actor->GetDistanceTo(TargetToFollow);
		if(tmpDist < DistanceToTarget)
		{
			DistanceToTarget = tmpDist;
			TargetToFollow = Actor;
		}
	}

	

	UBTFunctionLibrary::SetBlackboardValueAsFloat(this,DistanceToTargetSelector,DistanceToTarget);
	UBTFunctionLibrary::SetBlackboardValueAsObject(this,TargetToFollowSelector,TargetToFollow);
}
