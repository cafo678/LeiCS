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

	ActionComponent->OnLockedActorChangedDelegate.AddDynamic(this, &ALeiPlayerCharacter::OnLockedActorChanged);
}

void ALeiPlayerCharacter::OnLockedActorChanged(AActor* NewLockedActor)
{
	if (NewLockedActor)
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

	ensure(MyLeiPlayerController);

	return MyLeiPlayerController;
}

FVector ALeiPlayerCharacter::GetCameraLocation() const
{
	return CameraComponent->GetComponentLocation();
}


