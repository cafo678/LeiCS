// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/LeiAction.h"

#include "LeiBlueprintFunctionLibrary.h"
#include "Framework/LeiActionComponent.h"
#include "Framework/LeiAttributeSet.h"
#include "LeiCS/LeiCS.h"

DEFINE_LOG_CATEGORY(LogLeiAction);
DEFINE_LOG_CATEGORY(LogLeiTags);

void ULeiAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogLeiAction, Warning, TEXT("Running %s from %s"), *ActionTagID.ToString(), *GetNameSafe(GetOwningComponent()->GetOwner()));

	bIsRunning = true;
	
	ULeiActionComponent* ActionComponent = GetOwningComponent();

	if (StaminaCost && ActionComponent->AttributeSet->GetAttributeValue(TAG_Attribute_Stamina) >= StaminaCost)
	{
		ActionComponent->AttributeSet->ApplyAttributeChange(TAG_Attribute_Stamina, -StaminaCost);
	}	
	
	ActionComponent->ActiveGameplayTags.RemoveTags(RemoveTags);
	ActionComponent->ActiveGameplayTags.RemoveTags(RemoveTagsForever);
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTagsForever);

	UE_LOG(LogLeiTags, Warning, TEXT("%s Action %s removed tags: %s %s"), *GetNameSafe(ActionComponent->GetOwner()), *ActionTagID.ToString(), *RemoveTags.ToString(), *RemoveTagsForever.ToString());
	UE_LOG(LogLeiTags, Warning, TEXT("%s Action %s added tags: %s %s"), *GetNameSafe(ActionComponent->GetOwner()), *ActionTagID.ToString(), *GrantsTags.ToString(), *GrantsTagsForever.ToString());
}

void ULeiAction::StopAction_Implementation(AActor* Instigator)
{
	ULeiActionComponent* ActionComponent = GetOwningComponent();

	ensureAlways(bIsRunning);

	UE_LOG(LogLeiAction, Warning, TEXT("Stopping %s from %s"), *ActionTagID.ToString(), *GetNameSafe(ActionComponent->GetOwner()));

	if (bIsDirectional)
	{
		ensureAlways(ActionComponent->CurrentDirectionalActionDetails.Direction != TAG_Direction_None);

		ActionComponent->ResetCurrentDirectionalActionDetails();
	}

	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
	ActionComponent->ActiveGameplayTags.AppendTags(RemoveTags);

	UE_LOG(LogLeiTags, Warning, TEXT("%s Action %s readded tags removed: %s"), *GetNameSafe(ActionComponent->GetOwner()), *ActionTagID.ToString(), *RemoveTags.ToString());
	UE_LOG(LogLeiTags, Warning, TEXT("%s Action %s removed tags added: %s"), *GetNameSafe(ActionComponent->GetOwner()), *ActionTagID.ToString(), *GrantsTags.ToString());

	bIsRunning = false;
}

bool ULeiAction::CanStart_Implementation(AActor* Instigator)
{
	ULeiActionComponent* ActionComponent = GetOwningComponent();

	if ((StaminaCost && ActionComponent->AttributeSet->GetAttributeValue(TAG_Attribute_Stamina) < StaminaCost)  ||
		ActionComponent->ActiveGameplayTags.HasAny(BlockedTags)													||
		!ActionComponent->ActiveGameplayTags.HasAll(RequiredTags)												||
		bIsRunning)
	{
		return false;
	}

	return true;
}

bool ULeiAction::CanStop_Implementation(AActor* Instigator)
{
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
