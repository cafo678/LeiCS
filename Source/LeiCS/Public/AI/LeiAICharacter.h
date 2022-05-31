// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/LeiCharacter.h"
#include "LeiAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class LEICS_API ALeiAICharacter : public ALeiCharacter
{
	GENERATED_BODY()

public:
	ALeiAICharacter();
	
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Lei | AI | Components")
	UPawnSensingComponent* PawnSensingComponent = nullptr;

private:
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
