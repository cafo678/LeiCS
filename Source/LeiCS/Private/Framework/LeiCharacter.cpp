// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiCharacter.h"
#include "Framework/LeiActionComponent.h"
#include "MotionWarpingComponent.h"
#include "Framework/LeiAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"

ALeiCharacter::ALeiCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CharacterMovementComponent = GetCharacterMovement();
	
	SwordMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SwordMeshComponent");
	SwordMeshComponent->SetupAttachment(GetMesh(), "weapon_r");

	ActionComponent = CreateDefaultSubobject<ULeiActionComponent>("ActionComponent");

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
}

void ALeiCharacter::BeginPlay()
{
	Super::BeginPlay();

	static FGameplayTag MaxWalkSpeedAttributeTag = FGameplayTag::RequestGameplayTag("Attribute.MaxWalkSpeed");

	FOnAttributeChangedDelegate Delegate;
	Delegate.BindUFunction(this, "SetMaxWalkSpeed");
	
	ActionComponent->AttributeSet->AddAttributeChangedDelegate(MaxWalkSpeedAttributeTag, Delegate);
}

void ALeiCharacter::SetMaxWalkSpeed(float Value, float MaxValue, float MinValue)
{
	CharacterMovementComponent->MaxWalkSpeed = Value;
}

