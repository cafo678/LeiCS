// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiCharacter.h"
#include "Framework/LeiActionComponent.h"
#include "MotionWarpingComponent.h"
#include "Framework/LeiAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LeiTags.h"

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

	FOnAttributeChangedDelegate Delegate;
	Delegate.BindUFunction(this, "SetMaxWalkSpeed");

	ActionComponent->AttributeSet->AddAttributeChangedDelegate(TAG_Attribute_MaxWalkSpeed, Delegate);

	ActionComponent->OnActionStartedDelegate.AddDynamic(this, &ALeiCharacter::OnActionStarted);
	ActionComponent->OnActionStoppedDelegate.AddDynamic(this, &ALeiCharacter::OnActionStopped);
	ActionComponent->OnDamageReceivedDelegate.AddDynamic(this, &ALeiCharacter::OnDamageReceived);
	ActionComponent->OnParryReceivedDelegate.AddDynamic(this, &ALeiCharacter::OnParryReceived);
}

void ALeiCharacter::SetMaxWalkSpeed(float Value, float MaxValue, float MinValue)
{
	CharacterMovementComponent->MaxWalkSpeed = Value;
}

void ALeiCharacter::OnCombatSceneEntered_Implementation(AActor* Opponent)
{
	OnCombatSceneEnteredDelegate.Broadcast(Opponent);

	ActionComponent->SetOpponent(Opponent);

	ULeiActionComponent* OppononentActionComponent = ILeiActionComponentInterface::Execute_GetActionComponent(Opponent);
	OppononentActionComponent->OnActionStartedDelegate.AddDynamic(this, &ALeiCharacter::OnOpponentActionStarted);
	OppononentActionComponent->OnActionStoppedDelegate.AddDynamic(this, &ALeiCharacter::OnOpponentActionStopped);
}

void ALeiCharacter::OnActionStarted_Implementation(AActor* Character, FGameplayTag ActionTagID, FGameplayTag DirectionTag, bool bIsDirectional)
{
}

void ALeiCharacter::OnActionStopped_Implementation(AActor* Character, FGameplayTag ActionTagID, FGameplayTag DirectionTag, bool bIsDirectional)
{
}

void ALeiCharacter::OnOpponentActionStarted_Implementation(AActor* Opponent, FGameplayTag ActionTagID, FGameplayTag DirectionTag, bool bIsDirectional)
{
}

void ALeiCharacter::OnOpponentActionStopped_Implementation(AActor* Opponent, FGameplayTag ActionTagID, FGameplayTag DirectionTag, bool bIsDirectional)
{
}

void ALeiCharacter::OnDamageReceived_Implementation(float Delta, AActor* InInstigator, FGameplayTag InstigatorActionTagID)
{
}

void ALeiCharacter::OnParryReceived_Implementation(float BacklashDeltaLocation, float BacklashDuration)
{
}
