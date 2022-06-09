// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "LeiActionComponent.generated.h"

/** The action component will be responsible to handle all gameplay and combat related logic via Actions and Gameplay Tags */

DECLARE_LOG_CATEGORY_EXTERN(LogLeiAttributes, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockedActorChangedDelegate, AActor*, NewLockedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayStateChangedDelegate, FGameplayTag, NewStateTag);

class ULeiAction;
class ULeiAttributeSet;
class UDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEICS_API ULeiActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULeiActionComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ".Lei | Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ".Lei | Tags")
	FGameplayTag ActionDirection;

	UPROPERTY(BlueprintReadOnly, Category = ".Lei | Gameplay")
	AActor* LockedActor;
	
	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnLockedActorChangedDelegate OnLockedActorChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnGameplayStateChangedDelegate OnGameplayStateChangedDelegate;

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
	bool StartActionByTagID(AActor* Instigator, FGameplayTag ActionTagID);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Action")
	bool StopActionByTagID(AActor* Instigator, FGameplayTag ActionTagID);

	/** Attributes */

	UPROPERTY(EditAnywhere, Category = ".Lei | Attributes")
	TSubclassOf<ULeiAttributeSet> AttributeSetClass;

	UPROPERTY(BlueprintReadOnly, Category = ".Lei | Attributes")
	ULeiAttributeSet* AttributeSet = nullptr;

protected:
	UPROPERTY()
	TArray<ULeiAction*> Actions;
	
	UPROPERTY(EditAnywhere, Category = ".Lei | Action")
	TArray<TSubclassOf<ULeiAction>> DefaultActionsClasses;
};
