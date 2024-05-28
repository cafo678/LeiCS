// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiActionComponent.h"

#include "Framework/LeiAction.h"
#include "Framework/LeiAttributeSet.h"
#include "Framework/LeiComboHelper.h"

DEFINE_LOG_CATEGORY(LogLeiAttributes);

static TAutoConsoleVariable<bool> CVarDebugTags(TEXT("Lei.Debug.Tags"), true, TEXT("Enable the debug logging of gameplay tags"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugActions(TEXT("Lei.Debug.Actions"), false, TEXT("Enable the debug logging of Actions"), ECVF_Cheat);

ULeiActionComponent::ULeiActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GameplayState = TAG_GameplayState_Default;

	CurrentDirectionalActionDetails.ActionTagID = TAG_Action_None;
	CurrentDirectionalActionDetails.Direction = TAG_Direction_None;
}

void ULeiActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ComboHelper = NewObject<ULeiComboHelper>(this, ULeiComboHelper::StaticClass());

	for (const TSubclassOf<ULeiAction> ActionClass : DefaultActionsClasses)
	{
		AddAction(GetOwner(), ActionClass);
	}
	
	if (AttributeSetClass)
	{
		AttributeSet = NewObject<ULeiAttributeSet>(this, AttributeSetClass);
	}
	else
	{
		AttributeSet = NewObject<ULeiAttributeSet>(this, ULeiAttributeSet::StaticClass());
		
		UE_LOG(LogLeiAttributes, Warning, TEXT("Attribute Set not set in the Action Component, Attributes initialized to default"));
	}
}

void ULeiActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CVarDebugTags.GetValueOnGameThread())
	{
		const FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple() +
			"\nGameplayState: " + GameplayState.ToString() +
			"\nCurrentActionID: " + CurrentDirectionalActionDetails.ActionTagID.ToString() +
			"\nCurrentDirection: " + CurrentDirectionalActionDetails.Direction.ToString();
		
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, DebugMsg);
	}

	if (CVarDebugActions.GetValueOnGameThread())
	{
		for (const ULeiAction* Action : Actions)
		{
			const FString DebugMsg = GetNameSafe(GetOwner()) + " has action: " + Action->ActionTagID.ToString();
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, DebugMsg);
		}
	}
}

void ULeiActionComponent::AddAction(AActor* Instigator, TSubclassOf<ULeiAction> ActionClass)
{
	if (ensure(ActionClass))
	{
		for (const ULeiAction* Action : Actions)
		{
			if (Action->IsA(ActionClass))
			{
				if (CVarDebugActions.GetValueOnGameThread())
				{
					FString AddedMessage = FString::Printf(TEXT("Action of class: %s already in possession"), *GetNameSafe(ActionClass));
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, AddedMessage);
				}
				
				return;
			}
		}
		
		ULeiAction* NewAction = NewObject<ULeiAction>(this, ActionClass);
		
		if (ensure(NewAction) && !Actions.Contains(NewAction))
		{
			if (NewAction->ComboActionDetailsList.Num())
			{
				ComboHelper->RegisterComboActionInformation(NewAction->ActionTagID, NewAction->ComboActionDetailsList);
			}

			if (CVarDebugActions.GetValueOnGameThread())
			{
				const FString AddedMessage = FString::Printf(TEXT("Added Action: %s"), *NewAction->ActionTagID.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, AddedMessage);
			}
			
			Actions.Add(NewAction);

			if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
			{
				NewAction->StartAction(Instigator);
			}
		}
	}
}

void ULeiActionComponent::RemoveAction(ULeiAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	if (ActionToRemove->ComboActionDetailsList.Num())
	{
		ComboHelper->RemoveComboActionInformation(ActionToRemove->ActionTagID);
	}

	Actions.Remove(ActionToRemove);
}

void ULeiActionComponent::RemoveActionByTagID(FGameplayTag ActionTagID)
{
	for (ULeiAction* Action : Actions)
	{
		if (Action && Action->ActionTagID == ActionTagID)
		{
			RemoveAction(Action);

			return;
		}
	}
}

bool ULeiActionComponent::StartActionByTagID(AActor* Instigator, FGameplayTag ActionTagID, FGameplayTag DirectionTagID)
{	
	for (ULeiAction* Action : Actions)
	{
		if (Action && Action->ActionTagID == ActionTagID)
		{
			if (!Action->CanStart(Instigator))
			{
				UE_LOG(LogLeiAction, Warning, TEXT("Failed to run %s"), *ActionTagID.ToString());
				
				continue;
			}

			if (Action->bIsDirectional)
			{
				ensureAlways(DirectionTagID.IsValid() && DirectionTagID != TAG_Direction_None);

				CurrentDirectionalActionDetails.ActionTagID = ActionTagID;
				CurrentDirectionalActionDetails.Direction = DirectionTagID;
			}
			
			Action->StartAction(Instigator);

			OnActionStartedDelegate.Broadcast(ActionTagID, DirectionTagID);

			return true;
		}
	}
	
	return false;
}

bool ULeiActionComponent::StopActionByTagID(AActor* Instigator, FGameplayTag ActionTagID, EActionStopReason ActionStopReason)
{
	for (ULeiAction* Action : Actions)
	{
		if (Action && Action->ActionTagID == ActionTagID && Action->IsRunning())
		{
			if (!Action->CanStop(Instigator))
			{
				UE_LOG(LogLeiAction, Warning, TEXT("Failed to stop %s"), *ActionTagID.ToString());

				continue;
			}

			Action->StopAction(Instigator, ActionStopReason);

			return true;
		}
	}

	return false;
}

void ULeiActionComponent::ResetCurrentDirectionalActionDetails()
{
	CurrentDirectionalActionDetails.ActionTagID = TAG_Action_None;
	CurrentDirectionalActionDetails.Direction = TAG_Direction_None;

	OnResetCurrentDirectionalActionDetailsDelegate.Broadcast();
}

void ULeiActionComponent::SetOpponent(AActor* NewOpponent)
{
	Opponent = NewOpponent;
	OnOpponentSetDelegate.Broadcast(NewOpponent);
}

void ULeiActionComponent::OnGameplayStateChanged(FGameplayTag NewStateTag)
{
	OnGameplayStateChangedDelegate.Broadcast(NewStateTag);
}

