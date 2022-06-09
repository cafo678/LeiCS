// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "LeiAttributeSet.generated.h"

/** An attribute is everything that can be changed in its value from an action */
USTRUCT(BlueprintType)
struct LEICS_API FLeiAttribute
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = ".Lei | Attributes")
	float Value;

	UPROPERTY(EditAnywhere, Category = ".Lei | Attributes")
	float MaxValue;

	UPROPERTY(EditAnywhere, Category = ".Lei | Attributes")
	float MinValue;
};

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnAttributeChangedDelegate, float, Value, float, MaxValue, float, MinValue);

/** Collection of FLeiAttribute and functionalities to modify them, owned by an Action Component */
UCLASS(Blueprintable)
class LEICS_API ULeiAttributeSet : public UObject
{
	GENERATED_BODY()

public:
	ULeiAttributeSet();
	
	UFUNCTION(BlueprintCallable, Category = ".Lei | Attributes")
	bool ApplyAttributeChange(FGameplayTag AttributeTagID, float Delta);

	UFUNCTION(BlueprintPure, Category = ".Lei | Attributes")
	float GetAttributeValue(FGameplayTag AttributeTagID);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Attributes")
	void AddAttributeChangedDelegate(FGameplayTag AttributeTagID, FOnAttributeChangedDelegate OnAttributeChangedDelegate);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Attributes")
	void RemoveAttributeChangedDelegate(FGameplayTag AttributeTagID, FOnAttributeChangedDelegate OnAttributeChangedDelegate);

protected:
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Attributes")
	FLeiAttribute Health;

	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Attributes")
	FLeiAttribute Stamina;

	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Attributes")
	FLeiAttribute MaxWalkSpeed;

	/** Speed fot the preparation Notify State of an action montage */
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Attributes")
	FLeiAttribute PreparationSpeed;
	
private:
	TMultiMap<FGameplayTag, FOnAttributeChangedDelegate> AttributeChangedDelegatesMap;

	FLeiAttribute* GetAttributeByTag(FGameplayTag AttributeTag);
};


