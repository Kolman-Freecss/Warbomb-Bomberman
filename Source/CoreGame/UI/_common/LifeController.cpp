// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeController.h"

#include "Animation/AnimInstance.h"
#include "Components/WidgetComponent.h"
#include "CoreGame/WarbombPrivateSystems/packages/KolmanFreecss/Core/_common/BaseLifeController.h"

void UTC_LifeController::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetProgresBarProgress(1.f);
}

void UTC_LifeController::OnReciveDamage(float Damage, AActor* Offender, UBaseLifeController* Target)
{
	BP_OnReciveDamage(Damage);
	float Percent = (float)Target->Life / Target->MaxLife;
	SetProgresBarProgress(Percent);
	UE_LOG(LogTemp, Warning, TEXT("LifeController: %f damage, Instigator: %s"), Damage, *Offender->GetName());
}

void UTC_LifeController::SetProgresBarProgress(float Progress) const
{
	if (LifeBar)
	{
		LifeBar->SetPercent(Progress);
	}
}
