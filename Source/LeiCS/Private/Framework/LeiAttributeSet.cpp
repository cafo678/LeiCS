﻿
#include "Framework/LeiAttributeSet.h"

ULeiAttributeSet::ULeiAttributeSet()
{
	Health.Value = 100.f;
	Health.MaxValue = 100.f;
	Health.MinValue = 0.f;

	Stamina.Value = 100.f;
	Stamina.MaxValue = 100.f;
	Stamina.MinValue = 0.f;

	MaxWalkSpeed.Value = 600.f;
	MaxWalkSpeed.MaxValue = 900.f;
	MaxWalkSpeed.MinValue = 0.f;
	
	PreparationSpeed.Value = 1.f;
	PreparationSpeed.MaxValue = 2.f;
	PreparationSpeed.MinValue = 0.f;
}

FLeiAttribute* ULeiAttributeSet::GetAttributeByTag(FGameplayTag AttributeTag)
{
	if (AttributeTag.IsValid())
	{
		FString LeftString, RightString;
		const FString AttributeTagString = AttributeTag.ToString();
	
		AttributeTagString.Split(TEXT("."), &LeftString, &RightString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		for (TFieldIterator<FStructProperty> It(StaticClass()); It; ++It)
		{        
			FStructProperty* StructProp = *It;

			if (*StructProp->GetName() == FName(*RightString))
			{
				FLeiAttribute* Attribute = StructProp->ContainerPtrToValuePtr<FLeiAttribute>(this);

				return Attribute;
			}
		}
	}
	
	return nullptr;
}

bool ULeiAttributeSet::ApplyAttributeChange(FGameplayTag AttributeTagID, float Delta)
{
	FLeiAttribute* Attribute = GetAttributeByTag(AttributeTagID);

	if (Attribute)
	{
		Attribute->Value = FMath::Clamp(Attribute->Value + Delta, Attribute->MinValue, Attribute->MaxValue);

		for (const auto& AttributeChangedDelegates : AttributeChangedDelegatesMap)
		{
			if (AttributeChangedDelegates.Key == AttributeTagID && ensureAlways(AttributeChangedDelegates.Value.IsBound()))
			{
				AttributeChangedDelegates.Value.Execute(Attribute->Value, Attribute->MaxValue, Attribute->MinValue);
			}
		}

		return true;
	}

	return false;
}

float ULeiAttributeSet::GetAttributeValue(FGameplayTag AttributeTagID)
{
	FLeiAttribute* Attribute = GetAttributeByTag(AttributeTagID);

	if (Attribute)
	{
		return Attribute->Value;
	}

	return BIG_NUMBER;
}

void ULeiAttributeSet::AddAttributeChangedDelegate(FGameplayTag AttributeTagID, FOnAttributeChangedDelegate OnAttributeChangedDelegate)
{
	if (AttributeTagID.IsValid())
	{
		AttributeChangedDelegatesMap.Add(AttributeTagID, OnAttributeChangedDelegate);
	}
}

void ULeiAttributeSet::RemoveAttributeChangedDelegate(FGameplayTag AttributeTagID, FOnAttributeChangedDelegate OnAttributeChangedDelegate)
{
	if (AttributeTagID.IsValid())
	{
		AttributeChangedDelegatesMap.RemoveSingle(AttributeTagID, OnAttributeChangedDelegate);
	}
}

