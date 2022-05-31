// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LeiAICharacter.h"

#include "Framework/LeiActionComponent.h"
#include "Perception/PawnSensingComponent.h"

ALeiAICharacter::ALeiAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
}

void ALeiAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ALeiAICharacter::OnPawnSeen);
}

void ALeiAICharacter::OnPawnSeen(APawn* Pawn)
{
	ActionComponent->StartActionByName(Pawn, "EnterCombatState");
}
