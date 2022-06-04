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
	// TODO: Potrebbe essere utile tenere comunque il by name mettere in BP
	//ActionComponent->StartActionByName(Pawn, "EnterCombatState");
}
