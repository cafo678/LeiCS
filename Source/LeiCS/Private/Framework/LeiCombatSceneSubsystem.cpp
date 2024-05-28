// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/LeiCombatSceneSubsystem.h"
#include "Framework/LeiActionComponentInterface.h"
#include "Framework/LeiActionComponent.h"
#include "LeiTags.h"

void ULeiCombatSceneSubsystem::StartCombatScene(AActor* InPlayerCharacter, AActor* SceneEnemy)
{
	ensureAlways(!bIsCombatSceneActive && InPlayerCharacter && SceneEnemy && 
		InPlayerCharacter->Implements<ULeiActionComponentInterface>() && SceneEnemy->Implements<ULeiActionComponentInterface>());

	bIsCombatSceneActive = true;

	PlayerCharacter = InPlayerCharacter;
	CurrentSceneEnemy = SceneEnemy;

	ILeiActionComponentInterface::Execute_OnCombatSceneEntered(PlayerCharacter, CurrentSceneEnemy);
	ILeiActionComponentInterface::Execute_OnCombatSceneEntered(CurrentSceneEnemy, PlayerCharacter);
}

void ULeiCombatSceneSubsystem::EndCombatScene()
{
	ensureAlways(bIsCombatSceneActive && PlayerCharacter);

	CurrentSceneEnemy = nullptr;
	PlayerCharacter = nullptr;

	bIsCombatSceneActive = false;
}
