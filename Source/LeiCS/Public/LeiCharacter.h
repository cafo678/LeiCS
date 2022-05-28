// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LeiCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;


UCLASS()
class LEICS_API ALeiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALeiCharacter();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	void MoveForward(const float Value);

	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	void MoveRight(const float Value);

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent = nullptr;
};
