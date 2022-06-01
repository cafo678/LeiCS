// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiCharacter.h"
#include "Framework/LeiActionComponent.h"

ALeiCharacter::ALeiCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CharacterMovementComponent = GetCharacterMovement();
	
	SwordMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SwordMeshComponent");
	SwordMeshComponent->SetupAttachment(GetMesh(), "weapon_r");

	ActionComponent = CreateDefaultSubobject<ULeiActionComponent>("ActionComponent");
}

