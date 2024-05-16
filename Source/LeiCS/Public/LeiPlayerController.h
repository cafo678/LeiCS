// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "LeiPlayerController.generated.h"

class UInputMappingContext;
class ALeiPlayerCharacter;

UCLASS()
class LEICS_API ALeiPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Input")
	UInputMappingContext* DefaultMappingContext = nullptr;

	/** Error tolerance for the right stick when computing directional input */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Input")
	float RightStickErrorTolerance;
	
	/** Speed for when the camera will do the longest route to lock the target */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Camera")
	float LongestCameraRotationSpeed;

	/** Speed for when the camera will do the shortest route to lock the target */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Camera")
	float ShortestCameraRotationSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = ".Lei | Gameplay")
	AActor* LockedActor;

	UFUNCTION(BlueprintCallable, Category = ".Lei | Locomotion")
	void MoveForward(const float Value);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Locomotion")
	void MoveRight(const float Value);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Input")
	void HandleRightStick(const float XValue, const float YValue);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Input")
	void OnRightStickReleased();

	UFUNCTION(BlueprintCallable, Category = ".Lei | Input")
	void CheckGameplayStateInput();

public:
	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Gameplay")
	void OnLockedActorChanged(AActor* NewLockedActor);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Gameplay")
	void OnGameplayStateChanged(FGameplayTag NewStateTag);

	/** Used to decide when to enter stop locomotion state in the AnimBP */
	UPROPERTY(BlueprintReadWrite, Category = ".Lei | Input")
	bool HasUserReleasedMovementInput;
	
	/** Used to decide what animation to play (run / walk) when entering start locomotion state in the AnimBP */
	UFUNCTION(BlueprintCallable, Category = ".Lei | Input")
	float GetMovementInputValue() const;

private:
	bool bCanFireNewAction = true;
	
	float ForwardInputValue, RightInputValue;

	TArray<UInputMappingContext*> AppliedInputMappingContext;

	void AddMappingContext(UInputMappingContext* InputMappingContext, int32 Priority);
	void SetupActionsInput();
	void StartActionByInput(const FInputActionInstance& ActionInstance);
	void StopActionByInput(const FInputActionInstance& ActionInstance);

	FGameplayTag GetCorrectStatePlayerIsInBasedOnInput() const;
	
	FGameplayTag GetDirectionalActionIDToDoBasedOnState(FGameplayTag GameplayState) const;
	bool IsGameplayStateActionKeyPressed(FGameplayTag GameplayStateToCheck) const;

	FGameplayTag GetInputDirectionTag(const float XValue, const float YValue) const;
	float GetCameraDesiredSpeedByDelta(float Delta) const;
};


