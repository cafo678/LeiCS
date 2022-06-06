// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "LeiAction.generated.h"

/*
 * An Action is everything that can change combat and gameplay tags, have its own logic, and can be activated if owned by an ActionComponent
 */

DECLARE_LOG_CATEGORY_EXTERN(LogLeiAction, Log, All);

class ULeiActionComponent;

UCLASS(Blueprintable)
class LEICS_API ULeiAction : public UObject
{
	GENERATED_BODY()

protected:
	// This action executed grants these tags that will be removed when the action is stopped
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Tags")
	FGameplayTagContainer GrantsTags;

	// This action executed grants these tags that will NOT be removed when the action is stopped
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Tags")
	FGameplayTagContainer GrantsTagsForever;

	// This action executed remove these tags that will be removed when the action is stopped
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Tags")
	FGameplayTagContainer RemoveTags;

	// This action executed removes these tags that will NOT be removed when the action is stopped
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Tags")
	FGameplayTagContainer RemoveTagsForever;

	// This action will be executed only if the ActionComponent has any of these tags
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Tags")
	FGameplayTagContainer BlockedTags;

	// This action will not be executed if the ActionComponent has any of these tags
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Tags")
	FGameplayTagContainer RequiredTags;
	
	UPROPERTY(BlueprintReadOnly, Category = ".Lei | Tags")
	FGameplayTagContainer ContextTags;

	UFUNCTION(BlueprintPure, Category = ".Lei | Action")
	ULeiActionComponent* GetOwningComponent() const;

	// Override to give a proper UWorld to a UObject
	virtual UWorld* GetWorld() const override;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Action")
	FGameplayTag ActionTagID;

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void StartAction(AActor* Instigator, FGameplayTagContainer Context);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ".Lei | Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintPure, Category = ".Lei | Action")
	bool IsRunning() const { return bIsRunning; }

private:
	bool bIsRunning = false;
};
