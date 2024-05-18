// Fill out your copyright notice in the Description page of Project Settings.

#include "LeiBlueprintFunctionLibrary.h"
#include "LeiCS/LeiCS.h"

FGameplayTag ULeiBlueprintFunctionLibrary::MakeTagFromStringArray(TArray<FString>& Strings)
{
	FString ActionTagString;

	for (const FString& String : Strings)
	{
		if (ActionTagString.IsEmpty())
		{
			ActionTagString = String;
		}
		else
		{
			ActionTagString = ActionTagString + TEXT(".") + String;
		}
	}
	
	const FName ActionTagName = FName(*ActionTagString);
	const FGameplayTag ActionTag = FGameplayTag::RequestGameplayTag(ActionTagName);

	return ActionTag;
}

FString ULeiBlueprintFunctionLibrary::GetAbsoluteTagString(FGameplayTag Tag)
{
	if (Tag.IsValid())
	{
		FString LeftString, RightString;
		const FString TagString = Tag.ToString();
	
		TagString.Split(TEXT("."), &LeftString, &RightString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		return RightString;
	}

	return TEXT("");
}

FGameplayTag ULeiBlueprintFunctionLibrary::GetActionTagIDFromGameplayState(FGameplayTag GameplayState)
{
	/** Make the action tag ID from the gameplay state */
	const FString AbsoluteStateString = ULeiBlueprintFunctionLibrary::GetAbsoluteTagString(GameplayState);
	const FString ActionID = FString::Printf(TEXT("Action.%sState"), *AbsoluteStateString);
	const FName ActionIDName = FName(*ActionID);

	return FGameplayTag::RequestGameplayTag(ActionIDName);
}

bool ULeiBlueprintFunctionLibrary::IsDirectionalActionInputAllowed(FGameplayTagContainer ActiveGameplayTags, FGameplayTag DirectionalActionToDo)
{
	/** Check that the action is allowed to start, if we are recovering from a parry we can only defend */
	const bool bCanProcessDirectionalInput = ActiveGameplayTags.HasTag(TAG_CanProcessDirectionalInput);
	const bool bIsRecoveringAfterParry = ActiveGameplayTags.HasTag(TAG_IsRecoveringAfterParry);
	const bool bIsActionAllowedDuringRecovery = DirectionalActionToDo == TAG_Action_Defense || DirectionalActionToDo == TAG_Action_Dodge;

	return bCanProcessDirectionalInput && (!bIsRecoveringAfterParry || (bIsRecoveringAfterParry && bIsActionAllowedDuringRecovery));
}
