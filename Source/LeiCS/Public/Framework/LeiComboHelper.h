// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LeiCommonTypes.h"
#include "LeiComboHelper.generated.h"

USTRUCT()
struct FDirectionalActionDetailsList
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	TSet<FDirectionalActionDetails> DirectionalActionDetailsList;
};

UCLASS()
class LEICS_API ULeiComboHelper : public UObject
{
	GENERATED_BODY()

public:
	void RegisterComboActionInformation(FGameplayTag ActionTagID, TSet<FDirectionalActionDetails> RequestedComboDetailsList);
	void RemoveComboActionInformation(FGameplayTag ActionTagID);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Combo")
	bool IsComboAllowed(FGameplayTag ActionToDo, FDirectionalActionDetails CurrentDirectionalActionDetails) const;

private:
	UPROPERTY()
	TMap<FGameplayTag, FDirectionalActionDetailsList> ComboMap;
};
