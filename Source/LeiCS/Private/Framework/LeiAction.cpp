// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiAction.h"

#include "Framework/LeiActionComponent.h"

DEFINE_LOG_CATEGORY(LogLeiAction);

void ULeiAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogLeiAction, Warning, TEXT("Running %s"), *ActionName.ToString());
	
	ULeiActionComponent* ActionComponent = GetOwningComponent();
	
	ActionComponent->ActiveGameplayTags.RemoveTags(RemoveTags);
	ActionComponent->ActiveGameplayTags.RemoveTags(RemoveTagsForever);
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTagsForever);
	
	bIsRunning = true;
}

void ULeiAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogLeiAction, Warning, TEXT("Stopping %s"), *ActionName.ToString());

	ensureAlways(bIsRunning);
	
	ULeiActionComponent* ActionComponent = GetOwningComponent();
	
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
	ActionComponent->ActiveGameplayTags.AppendTags(RemoveTags);

	bIsRunning = false;
}

bool ULeiAction::CanStart_Implementation(AActor* Instigator)
{
	const ULeiActionComponent* ActionComponent = GetOwningComponent();

	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags) || IsRunning())
	{
		return false;
	}

	return true;
}

ULeiActionComponent* ULeiAction::GetOwningComponent() const
{
	return Cast<ULeiActionComponent>(GetOuter());
}

UWorld* ULeiAction::GetWorld() const
{
	UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter());
	if (ActorComponent)
	{
		return ActorComponent->GetWorld();
	}

	return nullptr;
}