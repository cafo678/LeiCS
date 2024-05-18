// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LeiBlueprintFunctionLibrary.generated.h"

UCLASS()
class LEICS_API ULeiBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Make a tag with every string in the array, every index is a children */
	UFUNCTION(BlueprintPure, Category = ".Lei | Tags")
	static FGameplayTag MakeTagFromStringArray(UPARAM(ref) TArray<FString>& Strings);
	
	/** Get a string representation of the last part of the tag */
	UFUNCTION(BlueprintPure, Category = ".Lei | Tags")
	static FString GetAbsoluteTagString(FGameplayTag Tag);
	
	UFUNCTION(BlueprintPure, Category = ".Lei | Tags")
	static FGameplayTag GetTagFromName(FName TagName) { return FGameplayTag::RequestGameplayTag(TagName); }

	UFUNCTION(BlueprintPure, Category = ".Lei | Action")
	static FGameplayTag GetActionTagIDFromGameplayState(FGameplayTag GameplayState);

	UFUNCTION(BlueprintPure, Category = ".Lei | Action")
	static bool IsDirectionalActionInputAllowed(FGameplayTagContainer ActiveGameplayTags, FGameplayTag DirectionalActionToDo);
};
