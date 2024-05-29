// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LeiAIController.h"
#include "Framework/LeiCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Framework/LeiActionComponent.h"
#include "LeiTags.h"

void ALeiAIController::BeginPlay()
{
	Super::BeginPlay();

	ALeiCharacter* ControlledCharacter = Cast<ALeiCharacter>(GetPawn());
	ControlledCharacter->OnCombatSceneEnteredDelegate.AddDynamic(this, &ALeiAIController::OnCombatSceneEntered);
	ControlledCharacter->OnOpponentActionStartedDelegate.AddDynamic(this, &ALeiAIController::OnOpponentActionStarted);
}

void ALeiAIController::OnCombatSceneEntered_Implementation(AActor* Opponent)
{
	RunBehaviorTree(BehaviorTree);
	SetFocus(Opponent);
	GetBlackboardComponent()->SetValueAsObject("PlayerPawn", Opponent);
}

void ALeiAIController::OnOpponentActionStarted_Implementation(AActor* Opponent, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag)
{
}
