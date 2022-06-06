// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiAction.h"

#include "LeiBlueprintFunctionLibrary.h"
#include "Framework/LeiActionComponent.h"

DEFINE_LOG_CATEGORY(LogLeiAction);

void ULeiAction::StartAction_Implementation(AActor* Instigator, FGameplayTagContainer Context)
{
	UE_LOG(LogLeiAction, Warning, TEXT("Running %s"), *ActionTagID.ToString());

	ContextTags = Context;
	
	ULeiActionComponent* ActionComponent = GetOwningComponent();
	
	ULeiBlueprintFunctionLibrary::RemoveChildrenTagsFromContainer(RemoveTags, ActionComponent->ActiveGameplayTags);
	ULeiBlueprintFunctionLibrary::RemoveChildrenTagsFromContainer(RemoveTagsForever, ActionComponent->ActiveGameplayTags);

	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTagsForever);
	
	bIsRunning = true;
}

void ULeiAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogLeiAction, Warning, TEXT("Stopping %s"), *ActionTagID.ToString());

	ensureAlways(bIsRunning);
	
	ULeiActionComponent* ActionComponent = GetOwningComponent();
	
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
	ActionComponent->ActiveGameplayTags.AppendTags(RemoveTags);

	bIsRunning = false;
}

bool ULeiAction::CanStart_Implementation(AActor* Instigator)
{
	ULeiActionComponent* ActionComponent = GetOwningComponent();

	ULeiBlueprintFunctionLibrary::AddChildrenTagsToContainer(BlockedTags, ActionComponent->ActiveGameplayTags);

	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags) || IsRunning())
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
