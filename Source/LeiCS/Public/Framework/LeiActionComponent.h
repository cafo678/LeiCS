// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "LeiActionComponent.generated.h"

/*
 * The action component will be responsible to handle all gameplay and combat related logic via Actions and Gameplay Tags
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnteredBattleState, AActor*, Instigator);

class ULeiAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEICS_API ULeiActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULeiActionComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lei | Action")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY()
	FOnEnteredBattleState OnEnteredBattleState;

	UFUNCTION(BlueprintCallable, Category = "Lei | Action")
	void AddAction(TSubclassOf<ULeiAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Lei | Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Lei | Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);
	
protected:
	UPROPERTY()
	TArray<ULeiAction*> Actions;

	UPROPERTY(EditAnywhere, Category = "Lei | Action")
	TArray<TSubclassOf<ULeiAction>> DefaultActionsClasses;
};
