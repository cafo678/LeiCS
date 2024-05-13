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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =".Lei | AI")
	UBehaviorTree* BehaviorTree = nullptr;
};
