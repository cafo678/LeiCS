// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/ActionPicker.h"

void UActionPicker::GetNextActionDetails(FGameplayTag ActionDirection, FActionDetails& ActionDetails)
{
	if(ensureAlways(ActionTable))
	{
		const FString ContextString;
		TArray<FActionDetailsRow*> ActionTableRows;
		
		ActionTable->GetAllRows<FActionDetailsRow>(ContextString, ActionTableRows);

		for (const FActionDetailsRow* Row : ActionTableRows)
		{
			if (Row->NextDirection == ActionDirection && Row->PreviousDirection == PreviousActionDirection)
			{
				ActionDetails = Row->ActionDetails;

				break;
			}
		}
	}
}
