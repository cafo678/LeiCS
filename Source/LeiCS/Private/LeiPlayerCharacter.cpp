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

	ActionComponent->OnEnteredBattleStateDelegate.AddDynamic(this, &ALeiPlayerCharacter::OnEnteredBattleState);
	ActionComponent->OnEnteredBattleStateDelegate.AddDynamic(this, &ALeiPlayerCharacter::OnLockedActorChanged);
}

void ALeiPlayerCharacter::OnEnteredBattleState(AActor* ActorInstigator)
{
	// Change movement type
	bUseControllerRotationYaw = true;
	CharacterMovementComponent->bOrientRotationToMovement = false;
	
	GetLeiPlayerController()->OnEnteredBattleState(ActorInstigator);
}

void ALeiPlayerCharacter::OnLockedActorChanged(AActor* NewLockedActor)
{
	GetLeiPlayerController()->OnLockedActorChanged(NewLockedActor);
}

ALeiPlayerController* ALeiPlayerCharacter::GetLeiPlayerController() const
{
	ALeiPlayerController* MyLeiPlayerController = Cast<ALeiPlayerController>(GetController());

	ensure(MyLeiPlayerController);

	return MyLeiPlayerController;
}

FVector ALeiPlayerCharacter::GetCameraLocation() const
{
	return CameraComponent->GetComponentLocation();
}


