// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LeiAIController.generated.h"

class UBehaviorTree;

UCLASS()
class LEICS_API ALeiAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	/** This flag controls wheter the AI can react to the opponent actions -- Events in Controller BP */
	UPROPERTY(BlueprintReadWrite, Category = ".Lei | AI")
	bool bCanReact = true;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =".Lei | AI")
	UBehaviorTree* BehaviorTree = nullptr;

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Gameplay")
	void OnPawnCombatSceneEntered(AActor* Opponent);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void OnPawnActionStarted(AActor* ControlledPawn, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void OnPawnActionStopped(AActor* ControlledPawn, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void OnOpponentActionStarted(AActor* Opponent, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void OnOpponentActionStopped(AActor* Opponent, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional);
};
