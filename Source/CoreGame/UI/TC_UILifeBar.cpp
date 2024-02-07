// Fill out your copyright notice in the Description page of Project Settings.

#include "TC_UILifeBar.h"

#include "CoreGame/Character/CoreGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

void UTC_UILifeBar::NativeOnInitialized()
{
	if (ACoreGameCharacter* Character = GetCoreCharacter())
	{
		Character->OnPlayerReciveDamageEvent.AddUniqueDynamic(this, &UTC_UILifeBar::OnReciveDamage);
	}
}

ACoreGameCharacter* UTC_UILifeBar::GetCoreCharacter() const
{
	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		return Cast<ACoreGameCharacter>(Character);
	}
	return nullptr;
}
