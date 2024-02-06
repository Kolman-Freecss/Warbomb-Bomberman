// Fill out your copyright notice in the Description page of Project Settings.

#include "TC_UILifeBar.h"

#include "CoreGame/Character/CoreGameCharacter.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

void UTC_UILifeBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetProgresBarProgress(1.f);

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

void UTC_UILifeBar::OnReciveDamage(float Damage, float Percent)
{
	BP_OnReciveDamage(Damage);
	SetProgresBarProgress(Percent);
}

void UTC_UILifeBar::SetProgresBarProgress(float Progress) const
{
	if (LifeBar)
	{
		LifeBar->SetPercent(Progress);
	}
}
