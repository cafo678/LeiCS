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

	/* Components */
	
	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SwordMeshComponent = nullptr;

	/* Input Actions */

	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	void MoveForward(const float Value);

	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	void MoveRight(const float Value);

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	bool bIsCombat;

	// Used to decide when to enter stop locomotion state in the AnimBP
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	bool HasUserReleasedMovementInput;
	
	// Used to decide what animation to play when entering start locomotion state in the AnimBP
	UFUNCTION(BlueprintCallable, Category = "Input")
	float GetMovementInputValue() const;
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	ALeiPlayerController* GetLeiPlayerController() const;

private:
	float ForwardInputValue, RightInputValue;
	
	UFUNCTION(Exec)
	void LeiChangeCombatState() { bIsCombat = !bIsCombat; }
};
