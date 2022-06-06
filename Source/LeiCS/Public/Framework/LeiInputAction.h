// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "LeiInputAction.generated.h"

/**
 * Custom Input Action with a GameplayTag associated for automatically firing Actions (see LeiPlayerController.cpp)
 * and more custom functionalities
 */

UCLASS()
class LEICS_API ULeiInputAction : public UInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Action)
	FGameplayTag ActionTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Action)
	bool bStopActionOnRelease;
};
