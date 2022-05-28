// Fill out your copyright notice in the Description page of Project Settings.

#include "LeiCharacter.h"

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

void ALeiCharacter::MoveForward(const float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	AddMovementInput(ControlRotation.Vector(), Value);
}

void ALeiCharacter::MoveRight(const float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	// From Kismet Math Library GetRightVector()
	const FVector ControlRightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(ControlRightVector, Value);
}

void ALeiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

