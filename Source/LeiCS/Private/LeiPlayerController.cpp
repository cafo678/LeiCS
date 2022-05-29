// Fill out your copyright notice in the Description page of Project Settings.


#include "LeiPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void ALeiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(DefaultMappingContext))
	{
		AddMappingContext(DefaultMappingContext, 0);
	}
}

void ALeiPlayerController::AddMappingContext(const UInputMappingContext* InputMappingContext, int32 Priority) const
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, Priority);
	}
}
