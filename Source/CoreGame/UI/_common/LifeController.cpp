// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeController.h"

#include "Animation/AnimInstance.h"

void UTC_LifeController::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetProgresBarProgress(1.f);
}

UTC_LifeController::UTC_LifeController()
{
}

void UTC_LifeController::OnReciveDamage(float Damage, float Percent)
{
	BP_OnReciveDamage(Damage);
	SetProgresBarProgress(Percent);
}

void UTC_LifeController::SetProgresBarProgress(float Progress) const
{
	if (LifeBar)
	{
		LifeBar->SetPercent(Progress);
	}
}
