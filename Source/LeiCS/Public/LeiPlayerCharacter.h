// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/LeiCharacter.h"
#include "LeiPlayerCharacter.generated.h"

class ALeiPlayerController;

UCLASS()
class LEICS_API ALeiPlayerCharacter : public ALeiCharacter
{
	GENERATED_BODY()

public:
	ALeiPlayerCharacter();

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "Lei | Utility")
	ALeiPlayerController* GetLeiPlayerController() const;

	FVector GetCameraLocation() const;

protected:
	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lei | Components")
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lei | Components")
	UCameraComponent* CameraComponent = nullptr;
	
	UFUNCTION(Category = "Lei | Gameplay")
	void OnEnteredBattleState(AActor* ActorInstigator);

	UFUNCTION(Category = "Lei | Gameplay")
	void OnLockedActorChanged(AActor* NewLockedActor);
};
