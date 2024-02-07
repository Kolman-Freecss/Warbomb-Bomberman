// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeController.h"

#include "Animation/AnimInstance.h"
#include "Components/WidgetComponent.h"

void UTC_LifeController::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Life = MaxLife;

	SetProgresBarProgress(1.f);
}

void UTC_LifeController::OnReciveDamage(float Damage, AActor* Instigator)
{
	Life -= Damage;
	BP_OnReciveDamage(Damage);
	SetProgresBarProgress((float)Life / MaxLife);
	UE_LOG(LogTemp, Warning, TEXT("LifeController: %f damage, Instigator: %s"), Damage, *Instigator->GetName());
}

void UTC_LifeController::SetProgresBarProgress(float Progress) const
{
	if (LifeBar)
	{
		LifeBar->SetPercent(Progress);
	}
}
