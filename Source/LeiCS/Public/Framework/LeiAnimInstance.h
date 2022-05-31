// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LeiAnimInstance.generated.h"

UENUM(BlueprintType)
enum class ELeiLocomotionStatus : uint8
{
	Default,
	Combat,
	Sprinting
};

UCLASS()
class LEICS_API ULeiAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Lei | Locomotion")
	ELeiLocomotionStatus LocomotionStatus = ELeiLocomotionStatus::Default;
};
