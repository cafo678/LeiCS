// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiActionComponent.h"

#include "Framework/LeiAction.h"

static TAutoConsoleVariable<bool> CVarDebugTags(TEXT("Lei.Debug.Tags"), true, TEXT("Enable the debug logging of gameplay tags"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugActions(TEXT("Lei.Debug.Actions"), true, TEXT("Enable the debug logging of Actions"), ECVF_Cheat);

ULeiActionComponent::ULeiActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize GameplayState of every actor with an action component to default
	static FGameplayTag DefaultGameplayStateTag = FGameplayTag::RequestGameplayTag("GameplayState.Default");
	
	ActiveGameplayTags.AddTag(DefaultGameplayStateTag);
}

void ULeiActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<ULeiAction> ActionClass : DefaultActionsClasses)
	{
		AddAction(ActionClass);
	}
}

void ULeiActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CVarDebugTags.GetValueOnGameThread())
	{
		const FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMsg);
	}
}

void ULeiActionComponent::AddAction(TSubclassOf<ULeiAction> ActionClass)
{
	if (ensure(ActionClass))
	{
		ULeiAction* NewAction = NewObject<ULeiAction>(this, ActionClass);
		
		if (ensure(NewAction))
		{
			Actions.Add(NewAction);
		}
	}
}

bool ULeiActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (ULeiAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				if (CVarDebugActions.GetValueOnGameThread())
				{
					FString FailedMessage = FString::Printf(TEXT("Failed to run %s"), *ActionName.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FailedMessage);
				}
				
				continue;
			}
			
			Action->StartAction(Instigator);
			
			return true;
		}
	}

	return false;
}

bool ULeiActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (ULeiAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName && Action->IsRunning())
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

void ULeiActionComponent::OnEnteredBattleState(AActor* Instigator)
{
	if (!LockedActor)
	{
		LockedActor = Instigator;
		OnLockedActorChangedDelegate.Broadcast(Instigator);
	}
	
	OnEnteredBattleStateDelegate.Broadcast(Instigator);
}

