// Fill out your copyright notice in the Description page of Project Settings.

#include "TC_UIBombCounter.h"
#include "CoreGame/CoreGameCharacter.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

void UTC_UIBombCounter::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ACoreGameCharacter* Character = GetCoreCharacter())
	{
		// Bind to character event to update the UI
		Character->OnPlayerReciveBombEvent.AddUniqueDynamic(this, &UTC_UIBombCounter::OnReciveBomb);

		// Set the initial bombs amount.
		const int BombsQuantity = Character->GetBombsQuantity();
		const bool bWithAnimation = false;
		BP_OnReciveBomb(BombsQuantity, bWithAnimation);
	}
}

ACoreGameCharacter* UTC_UIBombCounter::GetCoreCharacter()
{
	if(ACharacter * Character = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		return Cast<ACoreGameCharacter>(Character);
	}
	return nullptr;
}

void UTC_UIBombCounter::OnReciveBomb(int Amount)
{
	BP_OnReciveBomb(Amount);
}