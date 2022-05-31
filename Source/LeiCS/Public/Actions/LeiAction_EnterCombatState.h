// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/LeiAction.h"
#include "LeiAction_EnterCombatState.generated.h"

UCLASS()
class LEICS_API ULeiAction_EnterCombatState : public ULeiAction
{
	GENERATED_BODY()

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
};
