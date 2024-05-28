// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedPlayerInput.h"
#include "LeiEnhancedPlayerInput.generated.h"

UCLASS()
class LEICS_API ULeiEnhancedPlayerInput : public UEnhancedPlayerInput
{
	GENERATED_BODY()

public:
	const TArray<FEnhancedActionKeyMapping>& GetEnhancedActionMappingsPublic() const { return GetEnhancedActionMappings(); }
	
};
