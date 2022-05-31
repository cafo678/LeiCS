// Fill out your copyright notice in the Description page of Project Settings.


#include "LeiPlayerCharacter.h"

#include "Framework/LeiActionComponent.h"

ALeiPlayerCharacter::ALeiPlayerCharacter()
{
}

void ALeiPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ActionComponent->OnEnteredBattleState.AddDynamic(this, &ALeiPlayerCharacter::OnEnteredBattleState);
}

void ALeiPlayerCharacter::MoveForward(const float Value)
{
	ForwardInputValue = Value;
	
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	AddMovementInput(ControlRotation.Vector(), Value);
}

void ALeiPlayerCharacter::MoveRight(const float Value)
{
	RightInputValue = Value;
	
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	// From Kismet Math Library GetRightVector()
	const FVector ControlRightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(ControlRightVector, Value);
}

void ALeiPlayerCharacter::OnEnteredBattleState_Implementation(AActor* ActorInstigator)
{
	UE_LOG(LogTemp, Warning, TEXT("YOOOO"));
}

float ALeiPlayerCharacter::GetMovementInputValue() const
{
	const float AbsForwardInputValue = FMath::Abs(ForwardInputValue);
	const float AbsRightInputValue = FMath::Abs(RightInputValue);

	return AbsForwardInputValue >= AbsRightInputValue ? AbsForwardInputValue : AbsRightInputValue;
}
