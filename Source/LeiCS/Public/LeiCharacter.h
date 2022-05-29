// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LeiCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ALeiPlayerController;
class UStaticMeshComponent;

UCLASS()
class LEICS_API ALeiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALeiCharacter();

protected:
	virtual void BeginPlay() override;

	/* Input Actions */

	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	void MoveForward(const float Value);

	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	void MoveRight(const float Value);

public:	
	virtual void Tick(float DeltaTime) override;

	/* Input Variables for the Anim BP */
	
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector Velocity;
	
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	bool bHasUserReleasedMovementInput;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	bool bIsCombat;
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	ALeiPlayerController* GetLeiPlayerController() const;

private:
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* SwordMeshComponent = nullptr;

	float ForwardInputValue, RightInputValue;
	
	UFUNCTION(Exec)
	void LeiChangeCombatState() { bIsCombat = !bIsCombat; }
};
