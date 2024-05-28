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
	void StartCombatScene(AActor* InPlayerCharacter, AActor* SceneEnemy);

	UFUNCTION(BlueprintCallable, Category = ".Lei | CombatScene")
	void EndCombatScene();

	UFUNCTION(BlueprintCallable, Category = ".Lei | CombatScene")
	bool IsCombatSceneActive() const { return bIsCombatSceneActive; }

private:
	UPROPERTY()
	AActor* PlayerCharacter = nullptr;

	UPROPERTY()
	AActor* CurrentSceneEnemy;

	bool bIsCombatSceneActive;
};
