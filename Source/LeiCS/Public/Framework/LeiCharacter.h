// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeiActionComponentInterface.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "LeiCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatSceneEnteredDelegate, AActor*, Opponent);

class UStaticMeshComponent;
class ULeiActionComponent;
class UMotionWarpingComponent;

UCLASS()
class LEICS_API ALeiCharacter : public ACharacter, public ILeiActionComponentInterface
{
	GENERATED_BODY()

public:
	ALeiCharacter();

	virtual ULeiActionComponent* GetActionComponent_Implementation() const override { return ActionComponent; }
	virtual void OnCombatSceneEntered_Implementation(AActor* Opponent) override;

	UPROPERTY(BlueprintAssignable, Category = ".Lei | Gameplay")
	FOnCombatSceneEnteredDelegate OnCombatSceneEnteredDelegate;

protected:
	virtual void BeginPlay() override;

	/* Components */
	
	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ".Lei | Components")
	UStaticMeshComponent* SwordMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ".Lei | Components")
	ULeiActionComponent* ActionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ".Lei | Components")
	UMotionWarpingComponent* MotionWarpingComponent = nullptr;

	UFUNCTION()
	void SetMaxWalkSpeed(float Value, float MaxValue, float MinValue);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void OnDamageReceived(float Delta, AActor* InInstigator, FGameplayTag InstigatorActionTagID);

	/** Actions */
	
	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void OnActionStarted(AActor* Character, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void OnActionStopped(AActor* Character, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void OnOpponentActionStarted(AActor* Opponent, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void OnOpponentActionStopped(AActor* Opponent, FGameplayTag ActionTagID, FGameplayTag ActionDirectionTag, bool bIsDirectional);

	UFUNCTION(BlueprintNativeEvent, Category = ".Lei | Action")
	void OnParryReceived(float BacklashDeltaLocation, float BacklashDuration);
};
