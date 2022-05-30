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

float ALeiCharacter::GetMovementInputValue() const
{
	const float AbsForwardInputValue = FMath::Abs(ForwardInputValue);
	const float AbsRightInputValue = FMath::Abs(RightInputValue);

	return AbsForwardInputValue >= AbsRightInputValue ? AbsForwardInputValue : AbsRightInputValue;
}

ALeiPlayerController* ALeiCharacter::GetLeiPlayerController() const
{
	ALeiPlayerController* MyPlayerController = Cast<ALeiPlayerController>(GetController());
	
	ensure(MyPlayerController);
	
	return MyPlayerController;
}

