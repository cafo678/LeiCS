// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/LeiCharacter.h"
#include "LeiPlayerCharacter.generated.h"

UCLASS()
class LEICS_API ALeiPlayerCharacter : public ALeiCharacter
{
	GENERATED_BODY()

public:
	ALeiPlayerCharacter();

	virtual void PostInitializeComponents() override;

protected:
	/* Input Actions */

	UFUNCTION(BlueprintCallable, Category = "Lei | Locomotion")
	void MoveForward(const float Value);

	UFUNCTION(BlueprintCallable, Category = "Lei | Locomotion")
	void MoveRight(const float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TestAI;

	UFUNCTION(BlueprintNativeEvent, Category = "Lei | Gameplay")
	void OnEnteredBattleState(AActor* ActorInstigator);

public:	
	// Used to decide when to enter stop locomotion state in the AnimBP
	UPROPERTY(BlueprintReadWrite, Category = "Lei | Input")
	bool HasUserReleasedMovementInput;
	
	// Used to decide what animation to play when entering start locomotion state in the AnimBP
	UFUNCTION(BlueprintCallable, Category = "Lei | Input")
	float GetMovementInputValue() const;

private:
	float ForwardInputValue, RightInputValue;
	
};
