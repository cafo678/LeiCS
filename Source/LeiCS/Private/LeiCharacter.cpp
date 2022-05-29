// Fill out your copyright notice in the Description page of Project Settings.

#include "LeiCharacter.h"

#include "LeiPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ALeiCharacter::ALeiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterMovementComponent = GetCharacterMovement();

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	SwordMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SwordMeshComponent");
	SwordMeshComponent->SetupAttachment(GetMesh(), "weapon_r");
}

void ALeiCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALeiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	 * Treat speed as the greater input movement axis value (Root Motion)
	 * We take absolute values as we always want positive speed
	 */
	const float AbsForwardInputValue = FMath::Abs(ForwardInputValue);
	const float AbsRightInputValue = FMath::Abs(RightInputValue);
	
	Speed = AbsForwardInputValue >= AbsRightInputValue ? AbsForwardInputValue : AbsRightInputValue;
	
	/*
	 * We want the velocity applied to the character to always be related to the control rotation
	 * So if we go "forward" we will always go towards where the camera is looking regardless of where the character is facing
	 * We can't use standard MovementComponent variables as we are using Root Motion
	 */

	// Grab the rotation to where we want to move
	const FVector InputDirection = FVector(ForwardInputValue, RightInputValue, 0.f);
	const FRotator InputRotation = InputDirection.Rotation();
	
	// Grab only the Yaw Rotation of the ControlRotation so we don't end up in weird poses
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;
	
	// Treat velocity based on the input movement values (Root Motion) + the control rotation
	Velocity = (InputRotation + ControlRotation).Vector();

	/*
	* Since we are in Root Motion the movement component can't orientate towards the movement, so we do that manually here
	* We want to orientate to the movement only when not in combat and only when the player moves the character
	* This way the character will keep the last rotation even if we change the camera look
	* If we are in combat we simply want the camera to always point towards the enemy and we don't bother with the character rotation as we will use strafing animations
	*/
	
	if (bIsCombat)
	{
		SetActorRotation(ControlRotation);
	}
	else if (!bHasUserReleasedMovementInput)
	{
		SetActorRotation(Velocity.ToOrientationRotator());
	}
}

void ALeiCharacter::MoveForward(const float Value)
{
	ForwardInputValue = Value;
	
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	AddMovementInput(ControlRotation.Vector(), Value);
}

void ALeiCharacter::MoveRight(const float Value)
{
	RightInputValue = Value;
	
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	// From Kismet Math Library GetRightVector()
	const FVector ControlRightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(ControlRightVector, Value);
}

ALeiPlayerController* ALeiCharacter::GetLeiPlayerController() const
{
	ALeiPlayerController* MyPlayerController = Cast<ALeiPlayerController>(GetController());
	
	ensure(MyPlayerController);
	
	return MyPlayerController;
}

