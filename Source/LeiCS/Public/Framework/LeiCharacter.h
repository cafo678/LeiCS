// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeiActionComponentInterface.h"
#include "GameFramework/Character.h"
#include "LeiCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ALeiPlayerController;
class UStaticMeshComponent;
class ULeiActionComponent;

UCLASS()
class LEICS_API ALeiCharacter : public ACharacter, public ILeiActionComponentInterface
{
	GENERATED_BODY()

public:
	ALeiCharacter();

	virtual ULeiActionComponent* GetActionComponent_Implementation() const override { return ActionComponent; }

protected:
	/* Components */
	
	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ".Lei | Components")
	UStaticMeshComponent* SwordMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ".Lei | Components")
	ULeiActionComponent* ActionComponent = nullptr;
};
