// Fill out your copyright notice in the Description page of Project Settings.


#include "LeiBlueprintFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// TODO: probably can be better, can I do it in a single trace?
AActor* ULeiBlueprintFunctionLibrary::GetLockTarget(AActor* PlayerPawn, float TraceMultiplier, FVector TraceShapeHalfSize,
	TArray<TEnumAsByte<EObjectTypeQuery>> ActorTypeToQuery, ETraceTypeQuery TraceTypeQueryChannel)
{
	const FVector TraceStartLocation = PlayerPawn->GetActorLocation();
	const FVector PLayerPawnForwardVector = PlayerPawn->GetActorForwardVector();
	const FVector TraceEndLocation = TraceStartLocation + (PLayerPawnForwardVector * TraceMultiplier);
	const FVector PlayerCameraLocation = UGameplayStatics::GetPlayerCameraManager(PlayerPawn, 0)->GetCameraLocation();
	
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> PawnsResults;

	// Find pawns
	bool bPawnsTrace = UKismetSystemLibrary::BoxTraceMultiForObjects(PlayerPawn, TraceStartLocation, TraceEndLocation, TraceShapeHalfSize, FRotator::ZeroRotator,
		ActorTypeToQuery, false, ActorsToIgnore, EDrawDebugTrace::Persistent, PawnsResults, true);

	if (!bPawnsTrace)
	{
		return nullptr;
	}
	
	TArray<AActor*> VisibleActors;
	FHitResult VisibilityResult;

	// Check which pawns are visible
	for (const FHitResult& Hit : PawnsResults)
	{
		bool bVisibilityTrace = UKismetSystemLibrary::LineTraceSingle(PlayerPawn, PlayerCameraLocation, Hit.Location, TraceTypeQueryChannel, false, ActorsToIgnore,
			EDrawDebugTrace::Persistent, VisibilityResult, true);

		if (!bVisibilityTrace)
		{
			VisibleActors.Add(Hit.GetActor());
		}
	}

	AActor* BestActorResult = nullptr;
	float BestDistance = 0.f;

	// Find the closest visible pawn to lock
	for (AActor* VisibleActor : VisibleActors)
	{
		float Distance = VisibleActor->GetDistanceTo(PlayerPawn);
		
		if (Distance < BestDistance || BestDistance == 0.f)
		{
			BestActorResult = VisibleActor;
			BestDistance = Distance;
		}
	}

	return BestActorResult;
}
