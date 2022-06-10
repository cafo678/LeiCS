// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ActionPicker.generated.h"

/** Struct that holds Action Details of an action */
USTRUCT(BlueprintType)
struct FActionDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarpingMinDistance;
};

/** Simple struct for a table row that keeps Montages of attack action */
USTRUCT()
struct FActionDetailsRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag PreviousDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag NextDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActionDetails ActionDetails;
};

class UDataTable;
class UAnimMontage;

/** Responsible to pick the right action to execute */
UCLASS(Blueprintable)
class LEICS_API UActionPicker : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = ".Lei | Combo")
	void GetNextActionDetails(FGameplayTag NextDirection, FActionDetails& ActionDetails);

	UFUNCTION(BlueprintCallable, Category = ".Lei | Combo")
	void SetPreviousActionDirection(FGameplayTag PreviousDirection) { PreviousActionDirection = PreviousDirection; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Combo")
	UDataTable* ActionTable;

private:
	FGameplayTag PreviousActionDirection;
};
