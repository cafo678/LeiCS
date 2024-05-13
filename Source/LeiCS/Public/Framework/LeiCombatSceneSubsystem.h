// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LeiCombatSceneSubsystem.generated.h"

UCLASS()
class LEICS_API ULeiCombatSceneSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = ".Lei | CombatScene")
	void StartCombatScene(AActor* InPlayerCharacter, TSet<AActor*> SceneEnemies);

	UFUNCTION(BlueprintCallable, Category = ".Lei | CombatScene")
	void EndCombatScene();

	UFUNCTION(BlueprintCallable, Category = ".Lei | CombatScene")
	bool IsCombatSceneActive() const { return bIsCombatSceneActive; }

	UFUNCTION(BlueprintCallable, Category = ".Lei | CombatScene")
	void NotifyActionStartedOnEnemies(FGameplayTag ActionTagID, FGameplayTag DirectionTag);

	UFUNCTION(BlueprintCallable, Category = ".Lei | CombatScene")
	AActor* GetTargetToLock() const;

private:
	UPROPERTY()
	AActor* PlayerCharacter = nullptr;

	UPROPERTY()
	TSet<AActor*> CurrentSceneEnemies;

	bool bIsCombatSceneActive;
};
