// Fill out your copyright notice in the Description page of Project Settings.


#include "LeiPlayerCharacter.h"

#include "LeiPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Framework/LeiActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ALeiPlayerCharacter::ALeiPlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ALeiPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ActionComponent->OnOpponentSetDelegate.AddDynamic(this, &ALeiPlayerCharacter::OnOpponentSet);
}

void ALeiPlayerCharacter::OnCombatSceneEntered_Implementation(AActor* Opponent)
{
	Super::OnCombatSceneEntered_Implementation(Opponent);

	ActionComponent->StartActionByTagID(this, TAG_Action_CombatState, FGameplayTag());
}

void ALeiPlayerCharacter::OnOpponentSet(AActor* NewOpponent)
{
	if (NewOpponent)
	{
		bUseControllerRotationYaw = true;
		CharacterMovementComponent->bOrientRotationToMovement = false;
	}
	else
	{
		bUseControllerRotationYaw = false;
		CharacterMovementComponent->bOrientRotationToMovement = true;
	}
}

ALeiPlayerController* ALeiPlayerCharacter::GetLeiPlayerController() const
{
	ALeiPlayerController* MyLeiPlayerController = Cast<ALeiPlayerController>(GetController());

	return MyLeiPlayerController;
}


