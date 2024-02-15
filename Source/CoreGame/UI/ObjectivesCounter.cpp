// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectivesCounter.h"

#include "CoreGame/WarbombPrivateSystems/packages/KolmanFreecss/Config/GameMode/CoreGameGameMode.h"
#include "Kismet/GameplayStatics.h"

void UObjectivesCounter::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GameMode = Cast<ACoreGameGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode != nullptr)
	{
		GameMode->OnObjectiveCompletedTriggerDelegate.AddUniqueDynamic(this, &UObjectivesCounter::OnObjectiveCompleted);
		UpdateCounter(GameMode->CurrentObjectivesCompleted, GameMode->ObjectivesToComplete);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode is nullptr"));
	}
}

void UObjectivesCounter::OnObjectiveCompleted()
{
	UpdateCounter(GameMode->CurrentObjectivesCompleted, GameMode->ObjectivesToComplete);
	BP_OnObjectiveCompleted();
}
