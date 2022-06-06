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
	UFUNCTION(BlueprintCallable, Category = ".Lei | Gameplay")
	static AActor* GetLockTarget(AActor* PlayerPawn, float TraceMultiplier, FVector TraceShapeHalfSize,
		TArray<TEnumAsByte<EObjectTypeQuery>> ActorTypeToQuery, ETraceTypeQuery TraceTypeQueryChannel);

	// Make a tag with every string in the array, every index is a children
	UFUNCTION(BlueprintPure, Category = ".Lei | Action")
	static FGameplayTag MakeTagFromStringArray(UPARAM(ref) TArray<FString>& Strings);

	// Adds to FirstContainer, for every of its tag, all children tags found in the SecondContainer
	UFUNCTION(BlueprintCallable, Category = ".Lei | Tags")
	static void AddChildrenTagsToContainer(UPARAM(ref) FGameplayTagContainer& FirstContainer, UPARAM(ref) FGameplayTagContainer& SecondContainer);

	// Removes from SecondContainer all tags in the FirstContainer, including children
	UFUNCTION(BlueprintCallable, Category = ".Lei | Tags")
	static void RemoveChildrenTagsFromContainer(UPARAM(ref) FGameplayTagContainer& FirstContainer, UPARAM(ref) FGameplayTagContainer& SecondContainer);

	// Get the first encountered children tag of the specified tag that is in the specified container
	UFUNCTION(BlueprintPure, Category = ".Lei | Tags")
	static FGameplayTag GetFirstChildrenTag(FGameplayTag Tag, UPARAM(ref) FGameplayTagContainer& Container);
	
	// Get a string representation of the last part of the tag
	UFUNCTION(BlueprintPure, Category = ".Lei | Tags")
	static FString GetAbsoluteTagString(FGameplayTag Tag);
};
