// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiAction.h"

#include "LeiBlueprintFunctionLibrary.h"
#include "Framework/LeiActionComponent.h"

DEFINE_LOG_CATEGORY(LogLeiAction);

void ULeiAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogLeiAction, Warning, TEXT("Running %s"), *ActionTagID.ToString());

	bIsRunning = true;
	
	ULeiActionComponent* ActionComponent = GetOwningComponent();
	
	ActionComponent->ActiveGameplayTags.RemoveTags(RemoveTags);
	ActionComponent->ActiveGameplayTags.RemoveTags(RemoveTagsForever);
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTagsForever);
}

void ULeiAction::StopAction_Implementation(AActor* Instigator)
{
	ULeiActionComponent* ActionComponent = GetOwningComponent();

	ensureAlways(bIsRunning);
	
	UE_LOG(LogLeiAction, Warning, TEXT("Stopping %s"), *ActionTagID.ToString());
	
	bIsRunning = false;

	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
	ActionComponent->ActiveGameplayTags.AppendTags(RemoveTags);
}

bool ULeiAction::CanStart_Implementation(AActor* Instigator)
{
	const ULeiActionComponent* ActionComponent = GetOwningComponent();
	
	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags) /**|| bIsRunning*/)
	{
		return false;
	}

	if (RequiredTags.Num() > 0)
	{
		if (ActionComponent->ActiveGameplayTags.HasAll(RequiredTags))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

ULeiActionComponent* ULeiAction::GetOwningComponent() const
{
	return Cast<ULeiActionComponent>(GetOuter());
}

UWorld* ULeiAction::GetWorld() const
{
	const UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter());
	
	if (ActorComponent)
	{
		return ActorComponent->GetWorld();
	}

	return nullptr;
}
