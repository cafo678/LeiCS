// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiActionComponent.h"

#include "Framework/LeiActionPicker.h"
#include "Framework/LeiAction.h"
#include "Framework/LeiAttributeSet.h"

DEFINE_LOG_CATEGORY(LogLeiAttributes);

static TAutoConsoleVariable<bool> CVarDebugTags(TEXT("Lei.Debug.Tags"), true, TEXT("Enable the debug logging of gameplay tags"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugActions(TEXT("Lei.Debug.Actions"), true, TEXT("Enable the debug logging of Actions"), ECVF_Cheat);

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

	FOnAttributeChangedDelegate Delegate;
	Delegate.BindUFunction(this, "OnPoiseChanged");

	AttributeSet->AddAttributeChangedDelegate(TAG_Attribute_Poise, Delegate);
}

void ULeiActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CVarDebugTags.GetValueOnGameThread())
	{
		const FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple() +
			"\nGameplayState: " + GameplayState.ToString() +
			"\nActionID: " + CurrentDirectionalActionDetails.ActionTagID.ToString() +
			"\nDirection: " + CurrentDirectionalActionDetails.Direction.ToString();
		
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

	Actions.Remove(ActionToRemove);
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

bool ULeiActionComponent::StopActionByTagID(AActor* Instigator, FGameplayTag ActionTagID)
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

			Action->StopAction(Instigator);

			return true;
		}
	}

	return false;
}

bool ULeiActionComponent::HasEnoughStaminaForAction(FGameplayTag ActionTagID) const
{
	for (ULeiAction* Action : Actions)
	{
		if (Action && Action->ActionTagID == ActionTagID)
		{
			return Action->GetStaminaCost() <= AttributeSet->GetAttributeValue(TAG_Attribute_Stamina);
		}
	}
	
	return false;
}

void ULeiActionComponent::ResetCurrentDirectionalActionDetails()
{
	CurrentDirectionalActionDetails.ActionTagID = TAG_Action_None;
	CurrentDirectionalActionDetails.Direction = TAG_Direction_None;

	OnResetCurrentDirectionalActionDetailsDelegate.Broadcast();

	/** Set a timer to refill stamina when the combo is ended */
	GetWorld()->GetTimerManager().SetTimer(EndComboHandle, FTimerDelegate::CreateLambda([this]
		{
			if (CurrentDirectionalActionDetails.ActionTagID == TAG_Action_None && CurrentDirectionalActionDetails.Direction == TAG_Direction_None)
			{
				OnComboEndedDelegate.Broadcast(AttributeSet->GetAttributeValue(TAG_Attribute_Stamina));
				AttributeSet->ApplyAttributeChange(TAG_Attribute_Stamina, BIG_NUMBER);
			}
		}), 1.f, false);
}

void ULeiActionComponent::OnPoiseChanged(float Value, float MaxValue, float MinValue)
{
	if (Value == 0.f)
	{
		StopActionByTagID(GetOwner(), TAG_Action_ReceiveHit);
		StartActionByTagID(GetOwner(), TAG_Action_ReceiveStagger, FGameplayTag());
	}
}

void ULeiActionComponent::OnLockedActorChanged(AActor* NewLockedActor)
{
	LockedActor = NewLockedActor;
	OnLockedActorChangedDelegate.Broadcast(NewLockedActor);
}

void ULeiActionComponent::OnGameplayStateChanged(FGameplayTag NewStateTag)
{
	OnGameplayStateChangedDelegate.Broadcast(NewStateTag);
}

