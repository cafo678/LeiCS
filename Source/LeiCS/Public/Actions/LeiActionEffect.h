// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/LeiAction.h"
#include "LeiActionEffect.generated.h"

/** An action with a costant effect over time */

UCLASS()
class LEICS_API ULeiActionEffect : public ULeiAction
{
	GENERATED_BODY()

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ".Lei | Effect")
	float Duration;

	/** Time between ticks to apply effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ".Lei | Effect")
	float Period;

	/** If the action has to be removed automatically after its effect is finished */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ".Lei | Effect")
	bool bAutoRemove;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
};
