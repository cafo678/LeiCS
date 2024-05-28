// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiCharacter.h"
#include "Framework/LeiActionComponent.h"
#include "MotionWarpingComponent.h"
#include "Framework/LeiAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LeiCS/LeiCS.h"

ALeiCharacter::ALeiCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CharacterMovementComponent = GetCharacterMovement();
	
	SwordMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SwordMeshComponent");
	SwordMeshComponent->SetupAttachment(GetMesh(), "weapon_r");

	ActionComponent = CreateDefaultSubobject<ULeiActionComponent>("ActionComponent");

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
}

void ALeiCharacter::OnCombatSceneEntered_Implementation(AActor* Opponent)
{
	OnCombatSceneEnteredDelegate.Broadcast(Opponent);

	ActionComponent->SetOpponent(Opponent);

	ULeiActionComponent* OppononentActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(Opponent);
	OppononentActionComponent->OnActionStartedDelegate.AddDynamic(this, &ALeiCharacter::OnOpponentActionStarted);
}

void ALeiCharacter::BeginPlay()
{
	Super::BeginPlay();

	FOnAttributeChangedDelegate Delegate;
	Delegate.BindUFunction(this, "SetMaxWalkSpeed");

	ActionComponent->AttributeSet->AddAttributeChangedDelegate(TAG_Attribute_MaxWalkSpeed, Delegate);
}

void ALeiCharacter::SetMaxWalkSpeed(float Value, float MaxValue, float MinValue)
{
	CharacterMovementComponent->MaxWalkSpeed = Value;
}

void ALeiCharacter::OnOpponentActionStarted_Implementation(FGameplayTag ActionTagID, FGameplayTag DirectionTag)
{
	OnOpponentActionStartedDelegate.Broadcast(ActionTagID, DirectionTag);
}

