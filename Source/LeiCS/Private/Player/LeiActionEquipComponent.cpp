// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/LeiActionEquipComponent.h"
#include "LeiTags.h"

FGameplayTag ULeiActionEquipComponent::GetEquippedAction(bool bLeftSet, FGameplayTag ActionDirection) const
{
	if (bLeftSet)
	{
		if (LeftRuneSet.Contains(ActionDirection))
		{
			return *LeftRuneSet.Find(ActionDirection);
		}
	}
	else
	{
		if (RightRuneSet.Contains(ActionDirection))
		{
			return *RightRuneSet.Find(ActionDirection);
		}
	}

	return TAG_Action_None;
}
