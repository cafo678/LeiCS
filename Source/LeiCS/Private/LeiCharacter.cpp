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
}

void ALeiCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALeiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We treat speed as the greater input movement axis value (Root Motion)
	Speed = FMath::Abs(ForwardInputValue) >= FMath::Abs(RightInputValue) ? FMath::Abs(ForwardInputValue) : FMath::Abs(RightInputValue);

	// We treat velocity based on the input movement values (Root Motion)
	Velocity = FVector(ForwardInputValue, RightInputValue, 0.f);

	// Since we are in Root Motion the movement component can't orientate towards the movement, so we do that manually here
	// We want to orientate to the movement only when not in combat
	if (!bIsCombat)
	{
		SetActorRelativeRotation(Velocity.ToOrientationRotator());
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

