// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/LeiAction_EnterCombatState.h"

#include "Framework/LeiActionComponent.h"

void ULeiAction_EnterCombatState::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	const ULeiActionComponent* OwningComponent = GetOwningComponent();

	// Here we call the delegate on the owning ActionComponent that will set camera and lock on system
	OwningComponent->OnEnteredBattleState.Broadcast(Instigator);
}
