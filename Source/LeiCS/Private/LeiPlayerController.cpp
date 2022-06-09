// Fill out your copyright notice in the Description page of Project Settings.


#include "LeiPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Framework/LeiInputAction.h"
#include "Framework/LeiActionComponent.h"
#include "Framework/LeiActionComponentInterface.h"
#include "Kismet/KismetMathLibrary.h"

static TAutoConsoleVariable<bool> CVarDebugGameplay(TEXT("Lei.Debug.Gameplay"), true, TEXT("Enable the debug logging of Gameplay"), ECVF_Cheat);

void ALeiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(DefaultMappingContext))
	{
		AddMappingContext(DefaultMappingContext, 0);
		SetupActionsInput();
	}

	if (GetPawn()->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());
		
		PawnActionComponent->OnLockedActorChangedDelegate.AddDynamic(this, &ALeiPlayerController::OnLockedActorChanged);
		PawnActionComponent->OnGameplayStateChangedDelegate.AddDynamic(this, &ALeiPlayerController::OnGameplayStateChanged);
	}
}

void ALeiPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    			
	/** Keep camera on locked actor if we have one (therefore we are in combat) */
	if (LockedActor)
	{
		const FVector LockedActorLocation = LockedActor->GetActorLocation();
		const FVector CameraLocation = PlayerCameraManager->GetCameraLocation();

		const FRotator DesiredCameraRotation = (LockedActorLocation - CameraLocation).Rotation();

		const float DeltaYaw = FMath::Abs(DesiredCameraRotation.Yaw - GetControlRotation().Yaw);
		const float DeltaPitch = FMath::Abs(DesiredCameraRotation.Pitch - GetControlRotation().Pitch);

		const float DesiredSpeedByYaw = GetCameraDesiredSpeedByDelta(DeltaYaw);
		const float DesiredSpeedByPitch = GetCameraDesiredSpeedByDelta(DeltaPitch);

		const float DesiredSpeed = DesiredSpeedByYaw >= DesiredSpeedByPitch ? DesiredSpeedByYaw : DesiredSpeedByPitch;
		
		const FRotator RotationToSet = FMath::RInterpTo(GetControlRotation(), DesiredCameraRotation, DeltaSeconds, DesiredSpeed);
			
		SetControlRotation(RotationToSet);
	}
}

void ALeiPlayerController::AddMappingContext(UInputMappingContext* InputMappingContext, int32 Priority)
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		AppliedInputMappingContext.Add(InputMappingContext);
		
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, Priority);
	}
}

void ALeiPlayerController::SetupActionsInput()
{
	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		const UInputMappingContext* CurrentMappingContext = AppliedInputMappingContext.Last();

		for (const FEnhancedActionKeyMapping& Mapping : CurrentMappingContext->GetMappings())
		{
			const UInputAction* Action = Mapping.Action;

			if (Action)
			{
				PlayerEnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this, &ALeiPlayerController::StartActionByInput);
				PlayerEnhancedInputComponent->BindAction(Action, ETriggerEvent::Completed, this, &ALeiPlayerController::StopActionByInput);
			}
		}
	}
}

void ALeiPlayerController::StartActionByInput(const FInputActionInstance& ActionInstance)
{
	const ULeiInputAction* Action = Cast<ULeiInputAction>(ActionInstance.GetSourceAction());

	if (Action && Action->ActionTag.IsValid() && GetPawn()->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());

		PawnActionComponent->StartActionByTagID(GetPawn(), Action->ActionTag);
	}
}

void ALeiPlayerController::StopActionByInput(const FInputActionInstance& ActionInstance)
{
	const ULeiInputAction* Action = Cast<ULeiInputAction>(ActionInstance.GetSourceAction());

	if (Action && Action->ActionTag.IsValid() && Action->bStopActionOnRelease && GetPawn()->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());
		
		PawnActionComponent->StopActionByTagID(GetPawn(), Action->ActionTag);
	}
}

void ALeiPlayerController::OnLockedActorChanged_Implementation(AActor* NewLockedActor)
{
	if (CVarDebugGameplay.GetValueOnGameThread())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, FString::Printf(TEXT("Locked Actor changed: %s"), *GetNameSafe(NewLockedActor)));
	}

	LockedActor = NewLockedActor;
}

void ALeiPlayerController::OnGameplayStateChanged_Implementation(FGameplayTag NewStateTag)
{
}

void ALeiPlayerController::MoveForward(const float Value)
{
	if (APawn* MyPawn = GetPawn())
	{
		ForwardInputValue = Value;
	
		FRotator Rotation = GetControlRotation();
		Rotation.Pitch = 0.f;
		Rotation.Roll = 0.f;

		MyPawn->AddMovementInput(Rotation.Vector(), Value);
	}
}

void ALeiPlayerController::MoveRight(const float Value)
{
	if (APawn* MyPawn = GetPawn())
	{
		RightInputValue = Value;
		
		FRotator Rotation = GetControlRotation();
		Rotation.Pitch = 0.f;
		Rotation.Roll = 0.f;

		/** From Kismet Math Library GetRightVector() */
		const FVector ControlRightVector = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		
		MyPawn->AddMovementInput(ControlRightVector, Value);
	}
}

float ALeiPlayerController::GetCameraDesiredSpeedByDelta(float Delta) const
{
	float DesiredSpeed;
	
	if (Delta >= 180.f)
	{
		DesiredSpeed = FMath::GetMappedRangeValueClamped(FVector2D(180.f, 360.f),
			FVector2D(LongestCameraRotationSpeed, ShortestCameraRotationSpeed), Delta);
	}
	else
	{
		DesiredSpeed = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 180.f),
			FVector2D(ShortestCameraRotationSpeed, LongestCameraRotationSpeed), Delta);
	}

	return DesiredSpeed;
}

float ALeiPlayerController::GetMovementInputValue() const
{
	const float AbsForwardInputValue = FMath::Abs(ForwardInputValue);
	const float AbsRightInputValue = FMath::Abs(RightInputValue);

	return AbsForwardInputValue >= AbsRightInputValue ? AbsForwardInputValue : AbsRightInputValue;
}
