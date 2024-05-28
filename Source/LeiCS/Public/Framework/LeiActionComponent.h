// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "LeiTags.h"
#include "LeiCommonTypes.h"
#include "LeiActionComponent.generated.h"

/** The action component will be responsible to handle all gameplay and combat related logic via Actions and Gameplay Tags */

DECLARE_LOG_CATEGORY_EXTERN(LogLeiAttributes, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetCurrentDirectionalActionDetailsDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpponentSetDelegate, AActor*, NewOpponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayStateChangedDelegate, FGameplayTag, NewStateTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStartedDelegate, FGameplayTag, ActionTagID, FGameplayTag, ActionDirectionTag);

class ULeiAction;
class ULeiAttributeSet;
class ULeiComboHelper;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEICS_API ULeiActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULeiActionComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ".Lei | Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(BlueprintReadWrite, Category = ".Lei | Tags")
	FDirectionalActionDetails CurrentDirectionalActionDetails;

	/** Gameplay State */

	UPROPERTY(BlueprintReadWrite, Category = ".Lei | Tags")
	FGameplayTag GameplayState;

	UFUNCTION(BlueprintCallable, Category = ".Lei | Gameplay")
	void OnGameplayStateChanged(FGameplayTag NewStateTag);

	/** Opponent */

	UPROPERTY(BlueprintReadOnly, Category = ".Lei | Gameplay")
	AActor* Opponent;

	UFUNCTION(BlueprintCallable, Category = ".Lei | Gameplay")
	void SetOpponent(AActor* NewOpponent);

	/** Delegates */
	
	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnResetCurrentDirectionalActionDetailsDelegate OnResetCurrentDirectionalActionDetailsDelegate;

	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnOpponentSetDelegate OnOpponentSetDelegate;

	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnGameplayStateChangedDelegate OnGameplayStateChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnActionStartedDelegate OnActionStartedDelegate;

	/** Actions */
	
	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	void AddAction(AActor* Instigator, TSubclassOf<ULeiAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	void RemoveAction(ULeiAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	void RemoveActionByTagID(FGameplayTag ActionTagID);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	bool StartActionByTagID(AActor* Instigator, FGameplayTag ActionTagID, FGameplayTag DirectionTagID);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	bool StopActionByTagID(AActor* Instigator, FGameplayTag ActionTagID, EActionStopReason ActionStopReason = EActionStopReason::None);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	void ResetCurrentDirectionalActionDetails();

	/** Attributes */

	UPROPERTY(EditAnywhere, Category = ".Lei | Attributes")
	TSubclassOf<ULeiAttributeSet> AttributeSetClass = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = ".Lei | Attributes")
	ULeiAttributeSet* AttributeSet = nullptr;

	/** Combo Helper */

	UPROPERTY(BlueprintReadOnly, Category = ".Lei | Attributes")
	ULeiComboHelper* ComboHelper = nullptr;

protected:
	UPROPERTY()
	TArray<ULeiAction*> Actions;
	
	UPROPERTY(EditAnywhere, Category = ".Lei | Action")
	TArray<TSubclassOf<ULeiAction>> DefaultActionsClasses;	
};
