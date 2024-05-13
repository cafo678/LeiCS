// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "LeiEnemyActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULeiEnemyActorInterface : public UInterface
{
	GENERATED_BODY()
};

class LEICS_API ILeiEnemyActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | AI")
	void OnCombatSceneEntered(AActor* PlayerCharacter, const TSet<AActor*>& CombatSceneEnemies);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | AI")
	void OnPlayerActionStarted(FGameplayTag ActionTagID, FGameplayTag DirectionTag);
};
