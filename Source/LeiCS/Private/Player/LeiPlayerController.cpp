// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LeiPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "LeiBlueprintFunctionLibrary.h"
#include "Framework/LeiInputAction.h"
#include "Framework/LeiActionComponent.h"
#include "Framework/LeiActionComponentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "LeiCS/LeiCS.h"
#include "LeiCommonTypes.h"
#include "Framework/LeiCharacter.h"
#include "Framework/LeiComboHelper.h"
#include "Player/LeiEnhancedPlayerInput.h"
#include "Player/LeiActionEquipComponent.h"
#include "Player/LeiPlayerCharacter.h"

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
		
		PawnActionComponent->OnOpponentSetDelegate.AddDynamic(this, &ALeiPlayerController::OnOpponentSet);
		PawnActionComponent->OnGameplayStateChangedDelegate.AddDynamic(this, &ALeiPlayerController::OnGameplayStateChanged);
		PawnActionComponent->OnActionStoppedDelegate.AddDynamic(this, &ALeiPlayerController::OnActionStopped);
	}

	ALeiCharacter* ControlledCharacter = Cast<ALeiCharacter>(GetPawn());
	ControlledCharacter->OnOpponentActionStartedDelegate.AddDynamic(this, &ALeiPlayerController::OnOpponentActionStarted);
}

void ALeiPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    			
	/** Keep camera on locked actor if we have one (therefore we are in combat) */
	if (Opponent)
	{
		const FVector OpponentLocation = Opponent->GetActorLocation();
		const FVector CameraLocation = PlayerCameraManager->GetCameraLocation();

		FRotator DesiredCameraRotation = (OpponentLocation - CameraLocation).Rotation();
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
			if (PawnActionComponent->GameplayState == TAG_GameplayState_Default || (PawnActionComponent->GameplayState == TAG_GameplayState_Combat && !Opponent))
			{
				AddYawInput(XValue);
				AddPitchInput(YValue);

				return;
			}

			/** Check if the player wants to change state */
			FGameplayTag GameplayStateToGo = GetCorrectStatePlayerIsInBasedOnInput();

			/** Get the direction the player has pressed */
			FGameplayTag ActionDirection = GetInputDirectionTag(XValue, YValue);
			if (ActionDirection == TAG_Direction_None) return;

			/** Build the new ActionID from the state the player wants to go */
			const FGameplayTag NewActionTagID = GetDirectionalActionIDToDo(GameplayStateToGo, ActionDirection);
			if (NewActionTagID == TAG_Action_None) return;

			/** Check we have a valid directional action input and use a DoOnce so we don't fire every ticking axis input */
			if (ULeiBlueprintFunctionLibrary::IsDirectionalActionInputAllowed(PawnActionComponent->ActiveGameplayTags, NewActionTagID) && bCanFireNewAction)
			{
				bCanFireNewAction = false;

				FDirectionalActionDetails CurrentActionDetails = PawnActionComponent->CurrentDirectionalActionDetails;

				/** If the action we want to do has combo restrictions check that we are allowed to do it */
				if (PawnActionComponent->ComboHelper->IsComboAllowed(NewActionTagID, CurrentActionDetails))
				{
					/** Stop the action if any and cotinue */
					if (CurrentActionDetails.ActionTagID != TAG_Action_None)
					{
						PawnActionComponent->StopActionByTagID(this, PawnActionComponent->CurrentDirectionalActionDetails.ActionTagID);
					}
				}
				else
				{
					return;
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

void ALeiPlayerController::OnActionStopped(AActor* ActionActor, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional)
{
	if (bIsDirectional)
	{
		CheckGameplayStateInput();
	}
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

void ALeiPlayerController::OnOpponentSet_Implementation(AActor* NewOpponent)
{
	if (CVarDebugGameplay.GetValueOnGameThread())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Opponent changed: %s"), *GetNameSafe(NewOpponent)));
	}

	Opponent = NewOpponent;
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

void ALeiPlayerController::OnOpponentActionStarted_Implementation(AActor* OpponentActor, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag)
{
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

	/** Check that the key relative to that action is still pressed */
	if (PawnActionComponent)
	{
		const FGameplayTag ActionIDToCheck = ULeiBlueprintFunctionLibrary::GetActionTagIDFromGameplayState(GameplayStateToCheck);

		const ULeiEnhancedPlayerInput* EnhancedPlayerInput = Cast<ULeiEnhancedPlayerInput>(PlayerInput);

		TArray<FKey> ActionKeys;

		for (const FEnhancedActionKeyMapping& Mapping : EnhancedPlayerInput->GetEnhancedActionMappingsPublic())
		{
			const ULeiInputAction* Action = Cast<ULeiInputAction>(Mapping.Action);

			if (Action && Action->ActionTag == ActionIDToCheck)
			{
				ActionKeys.AddUnique(Mapping.Key);
			}
		}

		for (const FKey ActionKey : ActionKeys)
		{
			if (IsInputKeyDown(ActionKey))
			{
				return true;
			}
		}

		return false;

	}

	return true;
}

FGameplayTag ALeiPlayerController::GetDirectionalActionIDToDo(FGameplayTag GameplayState, FGameplayTag ActionDirection) const
{
	FString ActionString;

	if (GameplayState == TAG_GameplayState_Combat)
	{
		ActionString = TEXT("Dodge");
	}
	
	if (GameplayState == TAG_GameplayState_Rune)
	{
		const ALeiPlayerCharacter* LeiPlayerCharacter = Cast<ALeiPlayerCharacter>(GetPawn());
		const ULeiActionEquipComponent* ActionEquipComponent = LeiPlayerCharacter->GetActionEquipComponent();

		if (IsInputKeyDown(EKeys::Gamepad_LeftTrigger))
		{
			return ActionEquipComponent->GetEquippedAction(true, ActionDirection);
		}

		if (IsInputKeyDown(EKeys::Gamepad_RightTrigger))
		{
			return ActionEquipComponent->GetEquippedAction(false, ActionDirection);
		}
	}

	if (GameplayState == TAG_GameplayState_Attack || GameplayState == TAG_GameplayState_Defense)
	{
		ActionString = ULeiBlueprintFunctionLibrary::GetAbsoluteTagString(GameplayState);
	}

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
