// Fill out your copyright notice in the Description page of Project Settings.


#include "LeiPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "LeiBlueprintFunctionLibrary.h"
#include "Framework/LeiInputAction.h"
#include "Framework/LeiActionComponent.h"
#include "Framework/LeiActionComponentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "LeiCS/LeiCS.h"

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
		PawnActionComponent->OnResetCurrentDirectionalActionDetailsDelegate.AddDynamic(this, &ALeiPlayerController::CheckGameplayStateInput);
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

		FRotator DesiredCameraRotation = (LockedActorLocation - CameraLocation).Rotation();
		DesiredCameraRotation.Pitch = 0.f;

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
	if (GetPawn()->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());

		if (PawnActionComponent->ActiveGameplayTags.HasTag(TAG_CanProcessInput))
		{
			const ULeiInputAction* Action = Cast<ULeiInputAction>(ActionInstance.GetSourceAction());

			if (Action && Action->ActionTag.IsValid())
			{
				PawnActionComponent->StartActionByTagID(this, Action->ActionTag, FGameplayTag());
			}
		}
	}
}

void ALeiPlayerController::StopActionByInput(const FInputActionInstance& ActionInstance)
{
	const ULeiInputAction* Action = Cast<ULeiInputAction>(ActionInstance.GetSourceAction());

	if (Action && Action->ActionTag.IsValid() && Action->bStopActionOnRelease && GetPawn()->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());
		
		PawnActionComponent->StopActionByTagID(this, Action->ActionTag);
	}
}

void ALeiPlayerController::MoveForward(const float Value)
{
	if (GetPawn()->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());

		if (PawnActionComponent->ActiveGameplayTags.HasTag(TAG_CanProcessInput) && PawnActionComponent->ActiveGameplayTags.HasTag(TAG_CanMove))
		{
			ForwardInputValue = Value;

			FRotator Rotation = GetControlRotation();
			Rotation.Pitch = 0.f;
			Rotation.Roll = 0.f;

			GetPawn()->AddMovementInput(Rotation.Vector(), Value);
		}
	}
}

void ALeiPlayerController::MoveRight(const float Value)
{
	if (GetPawn()->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());

		if (PawnActionComponent->ActiveGameplayTags.HasTag(TAG_CanProcessInput) && PawnActionComponent->ActiveGameplayTags.HasTag(TAG_CanMove))
		{
			RightInputValue = Value;

			FRotator Rotation = GetControlRotation();
			Rotation.Pitch = 0.f;
			Rotation.Roll = 0.f;

			/** From Kismet Math Library GetRightVector() */
			const FVector ControlRightVector = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

			GetPawn()->AddMovementInput(ControlRightVector, Value);
		}
	}
}

void ALeiPlayerController::HandleRightStick(const float XValue, const float YValue)
{
	if (GetPawn()->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());

		if (PawnActionComponent->ActiveGameplayTags.HasTag(TAG_CanProcessInput))
		{
			/** If default state or in combat but without any locked actor just move the camera */
			if (PawnActionComponent->GameplayState == TAG_GameplayState_Default || (PawnActionComponent->GameplayState == TAG_GameplayState_Combat && !LockedActor))
			{
				AddYawInput(XValue);
				AddPitchInput(YValue);

				return;
			}

			/** Check if the player wants to change state */
			FGameplayTag GameplayStateToGo = GetCorrectStatePlayerIsInBasedOnInput();

			/** Build the new ActionID from the state the player wants to go */
			const FGameplayTag NewActionTagID = GetDirectionalActionIDToDoBasedOnState(GameplayStateToGo);

			/** Check we have a valid directional action input and use a DoOnce so we don't fire every ticking axis input */
			if (ULeiBlueprintFunctionLibrary::IsDirectionalActionInputAllowed(PawnActionComponent->ActiveGameplayTags, NewActionTagID) && 
				GetInputDirectionTag(XValue, YValue) != TAG_Direction_None && bCanFireNewAction)
			{
				bCanFireNewAction = false;

				/** Stop the current action if any, that means we are in combo */
				FGameplayTag CurrentDirectionalActionID = PawnActionComponent->CurrentDirectionalActionDetails.ActionTagID;

				if (CurrentDirectionalActionID != TAG_Action_None)
				{
					PawnActionComponent->StopActionByTagID(this, PawnActionComponent->CurrentDirectionalActionDetails.ActionTagID);
				}

				/** If we have to change the gameplay state ... */
				if (GameplayStateToGo != PawnActionComponent->GameplayState)
				{
					/** Stop the current state */
					PawnActionComponent->StopActionByTagID(this, ULeiBlueprintFunctionLibrary::GetActionTagIDFromGameplayState(PawnActionComponent->GameplayState));

					if (GameplayStateToGo != TAG_GameplayState_Combat)
					{
						/** Start the correct state */
						PawnActionComponent->StartActionByTagID(this, ULeiBlueprintFunctionLibrary::GetActionTagIDFromGameplayState(GameplayStateToGo), FGameplayTag());
					}
				}

				/** Start new action */
				PawnActionComponent->StartActionByTagID(this, NewActionTagID, GetInputDirectionTag(XValue, YValue));
			}
		}
	}
}

void ALeiPlayerController::OnRightStickReleased()
{
	bCanFireNewAction = true;
}

FGameplayTag ALeiPlayerController::GetInputDirectionTag(const float XValue, const float YValue) const
{	
	if (FMath::IsNearlyEqual(XValue, 1.f, RightStickErrorTolerance))
	{
		return TAG_Direction_Right;
	}
	else if (FMath::IsNearlyEqual(XValue, -1.f, RightStickErrorTolerance))
	{
		return TAG_Direction_Left;
	}
	else if (FMath::IsNearlyEqual(YValue, -1.f, RightStickErrorTolerance))
	{
		return TAG_Direction_Up;
	}
	else if (FMath::IsNearlyEqual(YValue, 1.f, RightStickErrorTolerance))
	{
		return TAG_Direction_Down;
	}
	else
	{
		return TAG_Direction_None;
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

void ALeiPlayerController::OnLockedActorChanged_Implementation(AActor* NewLockedActor)
{
	if (CVarDebugGameplay.GetValueOnGameThread())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Locked Actor changed: %s"), *GetNameSafe(NewLockedActor)));
	}

	LockedActor = NewLockedActor;
}

void ALeiPlayerController::OnGameplayStateChanged_Implementation(FGameplayTag NewStateTag)
{
}

void ALeiPlayerController::CheckGameplayStateInput()
{
	ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());

	if (PawnActionComponent)
	{
		/** Check which state the player has released wants to be in and change it */
		FGameplayTag GameplayStateToGo = GetCorrectStatePlayerIsInBasedOnInput();

		if (GameplayStateToGo != PawnActionComponent->GameplayState)
		{
			PawnActionComponent->StopActionByTagID(this, ULeiBlueprintFunctionLibrary::GetActionTagIDFromGameplayState(PawnActionComponent->GameplayState));

			if (GameplayStateToGo != TAG_GameplayState_Combat)
			{
				PawnActionComponent->StartActionByTagID(this, ULeiBlueprintFunctionLibrary::GetActionTagIDFromGameplayState(GameplayStateToGo), FGameplayTag());
			}
		}
	}
}

FGameplayTag ALeiPlayerController::GetCorrectStatePlayerIsInBasedOnInput() const
{
	if (IsGameplayStateActionKeyPressed(TAG_GameplayState_Attack))
	{
		return TAG_GameplayState_Attack;
	}

	if (IsGameplayStateActionKeyPressed(TAG_GameplayState_Defense))
	{
		return TAG_GameplayState_Defense;
	}

	if (IsGameplayStateActionKeyPressed(TAG_GameplayState_Rune))
	{
		return TAG_GameplayState_Rune;
	}

	return TAG_GameplayState_Combat;
}

bool ALeiPlayerController::IsGameplayStateActionKeyPressed(FGameplayTag GameplayStateToCheck) const
{
	ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());

	if (PawnActionComponent)
	{
		const FGameplayTag ActionIDToCheck = ULeiBlueprintFunctionLibrary::GetActionTagIDFromGameplayState(GameplayStateToCheck);

		/** Check that the key relative to that action is still pressed */
		if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			const UInputMappingContext* CurrentMappingContext = AppliedInputMappingContext.Last();

			for (const FEnhancedActionKeyMapping& Mapping : CurrentMappingContext->GetMappings())
			{
				const ULeiInputAction* Action = Cast<ULeiInputAction>(Mapping.Action);

				if (Action && Action->ActionTag == ActionIDToCheck)
				{
					const FKey KeyToCheckPressing = Mapping.Key;

					return IsInputKeyDown(KeyToCheckPressing);
				}
			}
		}
	}

	return true;
}

FGameplayTag ALeiPlayerController::GetDirectionalActionIDToDoBasedOnState(FGameplayTag GameplayState) const
{
	const FString ActionString = GameplayState == TAG_GameplayState_Combat ? TEXT("Dodge") : ULeiBlueprintFunctionLibrary::GetAbsoluteTagString(GameplayState);

	TArray<FString> ActionIDTagStrings;
	ActionIDTagStrings.Add(TEXT("Action"));
	ActionIDTagStrings.Add(ActionString);

	return ULeiBlueprintFunctionLibrary::MakeTagFromStringArray(ActionIDTagStrings);
}

float ALeiPlayerController::GetMovementInputValue() const
{
	const float AbsForwardInputValue = FMath::Abs(ForwardInputValue);
	const float AbsRightInputValue = FMath::Abs(RightInputValue);

	return AbsForwardInputValue >= AbsRightInputValue ? AbsForwardInputValue : AbsRightInputValue;
}
