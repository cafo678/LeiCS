// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LeiCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ALeiPlayerController;
class UStaticMeshComponent;
class ULeiActionComponent;

UCLASS()
class LEICS_API ALeiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALeiCharacter();

protected:
	/* Components */
	
	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lei | Components")
	UStaticMeshComponent* SwordMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lei | Components")
	ULeiActionComponent* ActionComponent = nullptr;
};
