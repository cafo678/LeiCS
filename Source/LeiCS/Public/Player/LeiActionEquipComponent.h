 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "LeiActionEquipComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEICS_API ULeiActionEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FGameplayTag GetEquippedAction(bool bLeftSet, FGameplayTag ActionDirection) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Action")
	TMap<FGameplayTag, FGameplayTag> LeftRuneSet;

	UPROPERTY(EditDefaultsOnly, Category = ".Lei | Action")
	TMap<FGameplayTag, FGameplayTag> RightRuneSet;
};
