// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/LeiComboHelper.h"

void ULeiComboHelper::RegisterComboActionInformation(FGameplayTag ActionTagID, TSet<FDirectionalActionDetails> RequestedComboDetailsList)
{
	FDirectionalActionDetailsList DirectionalActionDetailsList;
	DirectionalActionDetailsList.DirectionalActionDetailsList = RequestedComboDetailsList;

	ComboMap.Add(ActionTagID, DirectionalActionDetailsList);
}

void ULeiComboHelper::RemoveComboActionInformation(FGameplayTag ActionTagID)
{
	ComboMap.Remove(ActionTagID);
}

bool ULeiComboHelper::IsComboAllowed(FGameplayTag ActionToDo, FDirectionalActionDetails CurrentDirectionalActionDetails) const
{
	if (ComboMap.Contains(ActionToDo))
	{
		TSet<FDirectionalActionDetails> DirectionalActionDetailsList = ComboMap[ActionToDo].DirectionalActionDetailsList;

		return DirectionalActionDetailsList.Contains(CurrentDirectionalActionDetails);
	}

	return true;
}