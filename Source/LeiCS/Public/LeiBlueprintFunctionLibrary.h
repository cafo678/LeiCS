// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LeiBlueprintFunctionLibrary.generated.h"

UCLASS()
class LEICS_API ULeiBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = ".Lei | Gameplay")
	static AActor* GetLockTarget(AActor* PlayerPawn, float TraceMultiplier, FVector TraceShapeHalfSize,
		TArray<TEnumAsByte<EObjectTypeQuery>> ActorTypeToQuery, ETraceTypeQuery TraceTypeQueryChannel);
};
