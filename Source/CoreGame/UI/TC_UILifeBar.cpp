// Fill out your copyright notice in the Description page of Project Settings.

#include "TC_UILifeBar.h"

#include "Animation/AnimInstance.h"
#include "CoreGame/WarbombPrivateSystems/packages/KolmanFreecss/Config/PlayerState/CoreGamePlayerState.h"

void UTC_UILifeBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UTC_UILifeBar::NativeConstruct()
{
	Super::NativeConstruct();
	if (ACoreGamePlayerState* PlayerState = Cast<
		ACoreGamePlayerState>(GetOwningPlayerState()))
	{
		PlayerState->OnCharacterDamaged.AddUniqueDynamic(this, &UTC_UILifeBar::OnReciveDamage);
	}
}
