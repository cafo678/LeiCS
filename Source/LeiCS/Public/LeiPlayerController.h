// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LeiPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class LEICS_API ALeiPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext = nullptr;

private:
	void AddMappingContext(const UInputMappingContext* InputMappingContext, int32 Priority) const;
};
