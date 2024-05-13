// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/LeiCombatSceneSubsystem.h"
#include "AI/LeiEnemyActorInterface.h"
#include "Framework/LeiActionComponentInterface.h"
#include "Framework/LeiActionComponent.h"
#include "LeiCS/LeiCS.h"

void ULeiCombatSceneSubsystem::StartCombatScene(AActor* InPlayerCharacter, TSet<AActor*> SceneEnemies)
{
	ensureAlways(!bIsCombatSceneActive && InPlayerCharacter && SceneEnemies.Num());

	bIsCombatSceneActive = true;

	PlayerCharacter = InPlayerCharacter;

	if (PlayerCharacter->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PlayerActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(PlayerCharacter);
		PlayerActionComponent->StartActionByTagID(PlayerCharacter, TAG_Action_CombatState, FGameplayTag());
		PlayerActionComponent->OnActionStartedDelegate.AddDynamic(this, &ULeiCombatSceneSubsystem::NotifyActionStartedOnEnemies);
	}

	CurrentSceneEnemies = SceneEnemies;

	for (AActor* Enemy : CurrentSceneEnemies)
	{
		if (Enemy->Implements<ULeiEnemyActorInterface>())
		{
			ILeiEnemyActorInterface::Execute_OnCombatSceneEntered(Enemy, PlayerCharacter, CurrentSceneEnemies);
		}
	}
}

void ULeiCombatSceneSubsystem::EndCombatScene()
{
	ensureAlways(bIsCombatSceneActive && PlayerCharacter);

	if (PlayerCharacter->Implements<ULeiActionComponentInterface>())
	{
		ULeiActionComponent* PlayerActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(PlayerCharacter);
		PlayerActionComponent->OnActionStartedDelegate.RemoveDynamic(this, &ULeiCombatSceneSubsystem::NotifyActionStartedOnEnemies);
	}

	CurrentSceneEnemies.Empty();
	PlayerCharacter = nullptr;

	bIsCombatSceneActive = false;
}

void ULeiCombatSceneSubsystem::NotifyActionStartedOnEnemies(FGameplayTag ActionTagID, FGameplayTag DirectionTag)
{
	ensureAlways(bIsCombatSceneActive && PlayerCharacter && CurrentSceneEnemies.Num());

	for (AActor* Enemy : CurrentSceneEnemies)
	{
		if (Enemy->Implements<ULeiEnemyActorInterface>())
		{
			ILeiEnemyActorInterface::Execute_OnPlayerActionStarted(Enemy, ActionTagID, DirectionTag);
		}
	}
}

AActor* ULeiCombatSceneSubsystem::GetTargetToLock() const
{
	// TODO

	if (CurrentSceneEnemies.Num())
	{
		for (AActor* Enemy : CurrentSceneEnemies)
		{
			return Enemy;
		}
	}

	return nullptr;
}
