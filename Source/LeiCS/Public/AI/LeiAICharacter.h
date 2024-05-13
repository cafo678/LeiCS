// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/LeiCharacter.h"
#include "LeiEnemyActorInterface.h"
#include "LeiAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class LEICS_API ALeiAICharacter : public ALeiCharacter, public ILeiEnemyActorInterface
{
	GENERATED_BODY()

public:
	ALeiAICharacter();

	virtual void OnActorLocked_Implementation(const AActor* LockedActor) {}
};
