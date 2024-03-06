// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
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

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); //用于设置鼠标锁定行为。EMouseLockMode::DoNotLock表示不锁定鼠标到视口，即鼠标可以在窗口外移动。
	InputModeData.SetHideCursorDuringCapture(false); //用于设置在捕获（例如按下鼠标左键并拖动）期间是否隐藏光标。
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
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
