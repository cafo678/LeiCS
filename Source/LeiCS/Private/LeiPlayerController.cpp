// Fill out your copyright notice in the Description page of Project Settings.


#include "LeiPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "LeiPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

static TAutoConsoleVariable<bool> CVarDebugGameplay(TEXT("Lei.Debug.Gameplay"), true, TEXT("Enable the debug logging of Gameplay"), ECVF_Cheat);

void ALeiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(DefaultMappingContext))
	{
		AddMappingContext(DefaultMappingContext, 0);
	}
}

void ALeiPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    			
	// Keep camera on locked actor if we have one (therefore we are in combat) 
	if (LockedActor)
	{
		const FVector LockedActorLocation = LockedActor->GetActorLocation();
		const FVector CameraLocation = GetLeiPlayerCharacter()->GetCameraLocation();

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

void ALeiPlayerController::AddMappingContext(const UInputMappingContext* InputMappingContext, int32 Priority) const
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, Priority);
	}
}

void ALeiPlayerController::OnEnteredBattleState_Implementation(AActor* ActorInstigator)
{
	if (CVarDebugGameplay.GetValueOnGameThread())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, FString::Printf(TEXT("Entered Battle State by: %s"), *GetNameSafe(ActorInstigator)));
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

		// From Kismet Math Library GetRightVector()
		const FVector ControlRightVector = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		
		MyPawn->AddMovementInput(ControlRightVector, Value);
	}
}

ALeiPlayerCharacter* ALeiPlayerController::GetLeiPlayerCharacter() const
{
	ALeiPlayerCharacter* MyLeiPlayerCharacter = Cast<ALeiPlayerCharacter>(GetPawn());

	return MyLeiPlayerCharacter ? MyLeiPlayerCharacter : nullptr;
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
