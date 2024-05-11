// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AttributesTagsInfo.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Character/AuraCharacter.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetComponent/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorRes;
	GetHitResultUnderCursor(ECC_Visibility,false,CursorRes);
	if(!CursorRes.bBlockingHit) return;

	lastCursor = thisCursor;
	thisCursor = Cast<IEnemyInterface>(CursorRes.GetActor()); 
 
	if(thisCursor != nullptr)
	{
		if(lastCursor != nullptr)
		{
			if(lastCursor != thisCursor)
			{
				lastCursor->unHighlight();
				thisCursor->Highlight();
			}
		}
		else
		{
			thisCursor->Highlight();
		}
	}
	else
	{
		if(lastCursor != nullptr)
		{
			lastCursor->unHighlight();
		}
	}
	
}

void AAuraPlayerController::SpawnDamageText(float InComingDamage, ACharacter* TargetCharacter, bool bCritic, bool bBlock)
{
	if(IsValid(TargetCharacter) && DamageTextCompClass)
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter,DamageTextCompClass);
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(InComingDamage,bCritic,bBlock);
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); //用于设置鼠标锁定行为。EMouseLockMode::DoNotLock表示不锁定鼠标到视口，即鼠标可以在窗口外移动。
	InputModeData.SetHideCursorDuringCapture(false); //用于设置在捕获（例如按下鼠标左键并拖动）期间是否隐藏光标。
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent *AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraInputComponent->BindAction(MoveViewAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::MoveView);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Started,this,&AAuraPlayerController::ShiftInputPressed);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&AAuraPlayerController::ShiftInputReleased);
	
	//模板参数UserClass、PressedFuncType、ReleasedFuncType和HeldFuncType根据传递给函数的参数自动推断出来。 &ThisClass::AbilityInputTagHeld 是函数指针
	AuraInputComponent->BindInputActions(AuraInputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag Tag)
{
	if(Tag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		IsTargetingSth = thisCursor? true:false;
		IsAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag Tag)
{
	if(!Tag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		if(GetASC())
		{
			AuraAbilitySystemComponent->AbilityInputTagReleased(Tag);
		}
		return; 
	}
	if(GetASC()) AuraAbilitySystemComponent->AbilityInputTagReleased(Tag);
	
	if(!IsTargetingSth && !bShiftIsPressed)
	{
		const APawn* ControlledPawn = GetPawn();
		if(FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			Spline->ClearSplinePoints();
			const UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(),CacheMoveLocation);
			if(NavigationPath)
			{
				for(FVector Point:NavigationPath->PathPoints)
				{
					Spline->AddSplinePoint(Point,ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(),Point,5.0f,5.0f,FColor::Green,false,3.f);
				}
				IsAutoRunning = true;
				CacheMoveLocation = NavigationPath->PathPoints[NavigationPath->PathPoints.Num() - 1];
			}
		}
		FollowTime = 0.f;
		IsTargetingSth = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag Tag)
{
	if(!Tag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		if(GetASC())
		{
			AuraAbilitySystemComponent->AbilityInputTagHeld(Tag);
		}
		return;
	}
	if(IsTargetingSth || bShiftIsPressed)
	{
		if(GetASC())
		{
			AuraAbilitySystemComponent->AbilityInputTagHeld(Tag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult HitResult;
		
		if(GetHitResultUnderCursor(ECC_Visibility,false,HitResult))
		{
			CacheMoveLocation = HitResult.ImpactPoint;
		}

		if(APawn* ControlledPawn = GetPawn())
		{
			const FVector MoveDest = (CacheMoveLocation - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(MoveDest);
		}
	}
}

void AAuraPlayerController::AutoRun()
{
	if(!IsAutoRunning) return;
	
	if(APawn* ControlledPawn = GetPawn())
	{
		if(IsAutoRunning)
		{
			const FVector ClosestPoint = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
			const FVector ClosestDir = Spline->FindDirectionClosestToWorldLocation(ClosestPoint,ESplineCoordinateSpace::World);
			ControlledPawn->AddMovementInput(ClosestDir);

			if((CacheMoveLocation - ClosestPoint).Length() < AutoRunAcceptanceRadius) IsAutoRunning = false;
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if(AuraAbilitySystemComponent == nullptr)
		AuraAbilitySystemComponent =  Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector2D = InputActionValue.Get<FVector2D>();
	const FRotator Rotator = GetControlRotation();
	const FRotator yawRotator(0,Rotator.Yaw,0);
	
	const FVector ForwardDirection = FRotationMatrix(yawRotator).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(yawRotator).GetUnitAxis(EAxis::Y);

	if(APawn *ControlledAPawn = GetPawn())
	{
		ControlledAPawn->AddMovementInput(ForwardDirection,InputAxisVector2D.Y);
		ControlledAPawn->AddMovementInput(RightDirection,InputAxisVector2D.X);
	}
		
}

void AAuraPlayerController::MoveView(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector2D = InputActionValue.Get<FVector2D>();

	if(APawn *ControlledAPawn = GetPawn())
	{
		ControlledAPawn->AddControllerPitchInput(InputAxisVector2D.Y);
		ControlledAPawn->AddControllerYawInput(InputAxisVector2D.X);
	}
}
