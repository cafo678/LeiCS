// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiCharacter.h"
#include "Framework/LeiActionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ALeiCharacter::ALeiCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CharacterMovementComponent = GetCharacterMovement();

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	SwordMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SwordMeshComponent");
	SwordMeshComponent->SetupAttachment(GetMesh(), "weapon_r");

	ActionComponent = CreateDefaultSubobject<ULeiActionComponent>("ActionComponent");
}

