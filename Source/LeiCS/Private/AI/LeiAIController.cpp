// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LeiAIController.h"
#include "Framework/LeiCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Framework/LeiActionComponent.h"
#include "LeiTags.h"

void ALeiAIController::BeginPlay()
{
	Super::BeginPlay();

	if (GetPawn()->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PawnActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(GetPawn());

		PawnActionComponent->OnActionStartedDelegate.AddDynamic(this, &ALeiAIController::OnPawnActionStarted);
		PawnActionComponent->OnActionStoppedDelegate.AddDynamic(this, &ALeiAIController::OnPawnActionStopped);
	}

	if (ALeiCharacter* ControlledCharacter = Cast<ALeiCharacter>(GetPawn()))
	{
		ControlledCharacter->OnCombatSceneEnteredDelegate.AddDynamic(this, &ALeiAIController::OnPawnCombatSceneEntered);
	}
}

void ALeiAIController::OnPawnCombatSceneEntered_Implementation(AActor* Opponent)
{
	RunBehaviorTree(BehaviorTree);
	SetFocus(Opponent);
	GetBlackboardComponent()->SetValueAsObject("PlayerPawn", Opponent);

	ULeiActionComponent* OppononentActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(Opponent);
	OppononentActionComponent->OnActionStartedDelegate.AddDynamic(this, &ALeiAIController::OnOpponentActionStarted);
	OppononentActionComponent->OnActionStoppedDelegate.AddDynamic(this, &ALeiAIController::OnOpponentActionStopped);
}

void ALeiAIController::OnPawnActionStarted_Implementation(AActor* ControlledPawn, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional)
{
}

void ALeiAIController::OnPawnActionStopped_Implementation(AActor* ControlledPawn, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional)
{
}

void ALeiAIController::OnOpponentActionStarted_Implementation(AActor* Opponent, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional)
{
}

void ALeiAIController::OnOpponentActionStopped_Implementation(AActor* Opponent, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional)
{
}
