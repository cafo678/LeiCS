// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "LeiCS/LeiCS.h"
#include "LeiActionComponent.generated.h"

/** The action component will be responsible to handle all gameplay and combat related logic via Actions and Gameplay Tags */

DECLARE_LOG_CATEGORY_EXTERN(LogLeiAttributes, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetCurrentDirectionalActionDetailsDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboEndedDelegate, float, StaminaValueWhenComboEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockedActorChangedDelegate, AActor*, NewLockedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayStateChangedDelegate, FGameplayTag, NewStateTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStartedDelegate, FGameplayTag, ActionTagID, FGameplayTag, ActionDirectionTag);

USTRUCT(BlueprintType)
struct FDirectionalActionDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActionTagID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Direction;
};

class ULeiAction;
class ULeiAttributeSet;

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

	UPROPERTY(BlueprintReadWrite, Category = ".Lei | Tags")
	FGameplayTag GameplayState;

	UPROPERTY(BlueprintReadOnly, Category = ".Lei | Gameplay")
	AActor* LockedActor;
	
	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnResetCurrentDirectionalActionDetailsDelegate OnResetCurrentDirectionalActionDetailsDelegate;

	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnLockedActorChangedDelegate OnLockedActorChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnGameplayStateChangedDelegate OnGameplayStateChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnActionStartedDelegate OnActionStartedDelegate;

	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnComboEndedDelegate OnComboEndedDelegate;

	UFUNCTION(BlueprintCallable, Category = ".Lei | Gameplay")
	void OnLockedActorChanged(AActor* NewLockedActor);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Gameplay")
	void OnGameplayStateChanged(FGameplayTag NewStateTag);

	/** Actions */
	
	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	void AddAction(AActor* Instigator, TSubclassOf<ULeiAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	void RemoveAction(ULeiAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	bool StartActionByTagID(AActor* Instigator, FGameplayTag ActionTagID, FGameplayTag DirectionTagID);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	bool StopActionByTagID(AActor* Instigator, FGameplayTag ActionTagID);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	bool HasEnoughStaminaForAction(FGameplayTag ActionTagID) const;

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	void ResetCurrentDirectionalActionDetails();

	/** Attributes */

	UPROPERTY(EditAnywhere, Category = ".Lei | Attributes")
	TSubclassOf<ULeiAttributeSet> AttributeSetClass = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = ".Lei | Attributes")
	ULeiAttributeSet* AttributeSet = nullptr;

	UFUNCTION()
	void OnPoiseChanged(float Value, float MaxValue, float MinValue);

protected:
	UPROPERTY()
	TArray<ULeiAction*> Actions;
	
	UPROPERTY(EditAnywhere, Category = ".Lei | Action")
	TArray<TSubclassOf<ULeiAction>> DefaultActionsClasses;	

	FTimerHandle EndComboHandle;
};
