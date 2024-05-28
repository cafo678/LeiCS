// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/LeiCharacter.h"
#include "LeiPlayerCharacter.generated.h"

class ALeiPlayerController;
class USpringArmComponent;
class UCameraComponent;
class ULeiActionEquipComponent;

UCLASS()
class LEICS_API ALeiPlayerCharacter : public ALeiCharacter
{
	GENERATED_BODY()

public:
	ALeiPlayerCharacter();

	virtual void PostInitializeComponents() override;
	virtual void OnCombatSceneEntered_Implementation(AActor* Opponent) override;

	UFUNCTION(BlueprintCallable, Category = ".Lei | Utility")
	ULeiActionEquipComponent* GetActionEquipComponent() const;

	UFUNCTION(BlueprintCallable, Category = ".Lei | Utility")
	ALeiPlayerController* GetLeiPlayerController() const;

protected:
	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ".Lei | Components")
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ".Lei | Components")
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ".Lei | Components")
	ULeiActionEquipComponent* ActionEquipComponent = nullptr;

	UFUNCTION(Category = ".Lei | Gameplay")
	void OnOpponentSet(AActor* NewOpponent);
};
