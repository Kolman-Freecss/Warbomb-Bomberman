// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeBarWidget.h"

#include "Animation/AnimInstance.h"
#include "Components/WidgetComponent.h"
#include "CoreGame/WarbombPrivateSystems/packages/KolmanFreecss/Core/_common/BaseLifeController.h"

void UTC_LifeBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetProgresBarProgress(1.f);
}

void UTC_LifeBarWidget::OnReciveDamage(float Damage, AActor* _Instigator, UBaseLifeController* Target)
{
	BP_OnReciveDamage(Damage);
	SetProgresBarProgress((float)Target->Life / Target->MaxLife);
}

void UTC_LifeBarWidget::SetProgresBarProgress(float Progress)
{
	if (LifeBar)
	{
		LifeBar->SetPercent(Progress);
	}
}
