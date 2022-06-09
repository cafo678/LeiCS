// Fill out your copyright notice in the Description page of Project Settings.


#include "LeiBlueprintFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

static TAutoConsoleVariable<bool> CVarDebugLockTrace(TEXT("Lei.Debug.LockTrace"), true, TEXT("Enable the debug trace of lock ability"), ECVF_Cheat);

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

	const EDrawDebugTrace::Type Debugtrace = CVarDebugLockTrace.GetValueOnGameThread() ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None;

	/** Find pawns */
	bool bPawnsTrace = UKismetSystemLibrary::BoxTraceMultiForObjects(PlayerPawn, TraceStartLocation, TraceEndLocation, TraceShapeHalfSize, FRotator::ZeroRotator,
		ActorTypeToQuery, false, ActorsToIgnore, Debugtrace, PawnsResults, true);

	if (!bPawnsTrace)
	{
		return nullptr;
	}
	
	TArray<AActor*> VisibleActors;
	FHitResult VisibilityResult;

	/** Check which pawns are visible */
	for (const FHitResult& Hit : PawnsResults)
	{
		bool bVisibilityTrace = UKismetSystemLibrary::LineTraceSingle(PlayerPawn, PlayerCameraLocation, Hit.Location, TraceTypeQueryChannel, false, ActorsToIgnore,
			Debugtrace, VisibilityResult, true);

		if (!bVisibilityTrace)
		{
			VisibleActors.Add(Hit.GetActor());
		}
	}

	AActor* BestActorResult = nullptr;
	float BestDistance = 0.f;

	/** Find the closest visible pawn to lock */
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

FGameplayTag ULeiBlueprintFunctionLibrary::MakeTagFromStringArray(TArray<FString>& Strings)
{
	FString ActionTagString;

	for (const FString& String : Strings)
	{
		if (ActionTagString.IsEmpty())
		{
			ActionTagString = String;
		}
		else
		{
			ActionTagString = ActionTagString + TEXT(".") + String;
		}
	}
	
	const FName ActionTagName = FName(*ActionTagString);
	const FGameplayTag ActionTag = FGameplayTag::RequestGameplayTag(ActionTagName);

	return ActionTag;
}

FGameplayTag ULeiBlueprintFunctionLibrary::GetFirstChildrenTag(FGameplayTag Tag, FGameplayTagContainer& Container)
{
	FGameplayTag ReturnTag;
	
	for (auto ContainerTag = Container.CreateConstIterator(); ContainerTag; ++ContainerTag)
	{
		if (ContainerTag->MatchesTag(Tag))
		{
			ReturnTag = *ContainerTag;
		}
	}

	return ReturnTag;
}

FString ULeiBlueprintFunctionLibrary::GetAbsoluteTagString(FGameplayTag Tag)
{
	if (Tag.IsValid())
	{
		FString LeftString, RightString;
		const FString TagString = Tag.ToString();
	
		TagString.Split(TEXT("."), &LeftString, &RightString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		return RightString;
	}

	return TEXT("");
}
