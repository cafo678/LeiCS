// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "LeiAction.generated.h"

/*
 * An Action is everything that can be executed from an ActionComponent
 * All input player based gameplay elements will be actions, and can be used by AI as well
 * It is ok to have base classes for similar actions but ultimately every action that differs on its core functionality (tags, input, different behaviour) should be its own class
 * These are nice little, standalone packed assets that can be checked out singularly and edited without have to bother other major assets (Character or Player Controller)
 * Actions apply Gameplay Effects to change Attributes
 */

DECLARE_LOG_CATEGORY_EXTERN(LogLeiAction, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogLeiTags, Log, All);

class ULeiActionComponent;

UCLASS(Blueprintable)
class LEICS_API ULeiAction : public UObject
{
	GENERATED_BODY()

protected:
	/** This action executed grants these tags that will be removed when the action is stopped */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Action | Tags")
	FGameplayTagContainer GrantsTags;

	/** This action executed grants these tags that will NOT be removed when the action is stopped */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Action | Tags")
	FGameplayTagContainer GrantsTagsForever;

	/** This action executed remove these tags that will be readded when the action is stopped */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Action | Tags")
	FGameplayTagContainer RemoveTags;

	/** This action executed removes these tags that will NOT be readded when the action is stopped */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Action | Tags")
	FGameplayTagContainer RemoveTagsForever;

	/** This action will not be executed if the ActionComponent has any of these tags */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Action | Tags")
	FGameplayTagContainer BlockedTags;
	
	/** This action will be executed only if the ActionComponent has any of these tags */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Action | Tags")
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Properties")
	float StaminaCost = 0.f;

	UFUNCTION(BlueprintPure, Category = ".Lei | Action")
	ULeiActionComponent* GetOwningComponent() const;

	/** Override to give a proper UWorld to a UObject */
	virtual UWorld* GetWorld() const override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ".Lei | Properties")
	FGameplayTag ActionTagID;
	
	/** This action will start as soon as added to an action component */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Properties")
	bool bAutoStart;
	
	/** This action is relative to a directional input */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Properties")
	bool bIsDirectional;

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	bool CanStop(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ".Lei | Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	bool IsRunning() const { return bIsRunning; }

private:
	bool bIsRunning = false;
};
