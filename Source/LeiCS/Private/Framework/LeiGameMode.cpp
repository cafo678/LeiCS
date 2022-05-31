// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/LeiGameMode.h"

void ALeiGameMode::StartPlay()
{
	Super::StartPlay();

	GEngine->Exec(GetWorld(), TEXT("t.maxFPS 60"));
	GEngine->Exec(GetWorld(), TEXT("stat unit"));
	GEngine->Exec(GetWorld(), TEXT("stat fps"));
}
