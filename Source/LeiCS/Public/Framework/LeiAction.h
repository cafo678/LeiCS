// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "LeiAction.generated.h"

/*
 * An Action is everything that can change combat and gameplay tags and can be activated if owned by an ActionComponent
 */

DECLARE_LOG_CATEGORY_EXTERN(LogLeiAction, Log, All);

class ULeiActionComponent;

UCLASS(Blueprintable)
class LEICS_API ULeiAction : public UObject
{
	GENERATED_BODY()

protected:
	// This action executed grants these tags that will be removed when the action is stopped
	UPROPERTY(EditDefaultsOnly, Category = "Lei | Tags")
	FGameplayTagContainer GrantsTags;

	// These won't be removed when the action is stopped
	UPROPERTY(EditDefaultsOnly, Category = "Lei | Tags")
	FGameplayTagContainer GrantsTagsForever;

	UPROPERTY(EditDefaultsOnly, Category = "Lei | Tags")
	FGameplayTagContainer RemoveTags;

	UPROPERTY(EditDefaultsOnly, Category = "Lei | Tags")
	FGameplayTagContainer RemoveTagsForever;

	// This action will not be executed if the ActionComponent has any of this tags
	UPROPERTY(EditDefaultsOnly, Category = "Lei | Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintPure, Category = "Lei | Action")
	ULeiActionComponent* GetOwningComponent() const;

	// Override to give a proper UWorld to a UObject
	virtual UWorld* GetWorld() const override;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Lei | Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Lei | Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Lei | Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Lei | Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintPure, Category = "Lei | Action")
	bool IsRunning() const { return bIsRunning; }

private:
	bool bIsRunning = false;
	
};
