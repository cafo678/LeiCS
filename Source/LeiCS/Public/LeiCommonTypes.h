#pragma once

#include "CoreMinimal.h"
#include "LeiCS/LeiCS.h"
#include "LeiCommonTypes.generated.h"

UENUM(BlueprintType)
enum class EActionStopReason : uint8
{
	None,
	Interrupted,
};

USTRUCT(BlueprintType)
struct FDirectionalActionDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActionTagID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Direction;

	friend uint32 GetTypeHash(const FDirectionalActionDetails& DirectionalActionDetails)
	{
		return HashCombine(GetTypeHash(DirectionalActionDetails.ActionTagID), GetTypeHash(DirectionalActionDetails.Direction));
	}

	FORCEINLINE bool operator==(const FDirectionalActionDetails& Other) const
	{
		return ActionTagID == Other.ActionTagID && Direction == Other.Direction;
	}
};