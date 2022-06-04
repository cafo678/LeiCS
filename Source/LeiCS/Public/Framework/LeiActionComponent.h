// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "LeiActionComponent.generated.h"

/*
 * The action component will be responsible to handle all gameplay and combat related logic via Actions and Gameplay Tags
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockedActorChangedDelegate, AActor*, NewLockedActor);

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
	FOnLockedActorChangedDelegate OnLockedActorChangedDelegate;

	UFUNCTION(BlueprintCallable, Category = "Lei | Action")
	void AddAction(TSubclassOf<ULeiAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Lei | Action")
	bool StartActionByTagID(AActor* Instigator, FGameplayTag ActionTagID);

	UFUNCTION(BlueprintCallable, Category = "Lei | Action")
	bool StopActionByTagID(AActor* Instigator, FGameplayTag ActionTagID);

	UFUNCTION(BlueprintCallable, Category = "Lei | Gameplay")
	void OnLockedActorChanged(AActor* NewLockedActor);
	
protected:
	UPROPERTY()
	TArray<ULeiAction*> Actions;

	UPROPERTY(EditAnywhere, Category = "Lei | Action")
	TArray<TSubclassOf<ULeiAction>> DefaultActionsClasses;

private:
	UPROPERTY()
	AActor* LockedActor;
};
