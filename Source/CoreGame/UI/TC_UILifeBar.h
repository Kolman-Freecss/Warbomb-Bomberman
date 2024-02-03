// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TC_UILifeBar.generated.h"

class UProgressBar;
class ACoreGameCharacter;

UCLASS()
class COREGAME_API UTC_UILifeBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeOnInitialized();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* LifeBar = nullptr;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnReciveDamage(float Damage);

private:

	UFUNCTION()
	void OnReciveDamage(float Damage, float Percent);
	void SetProgresBarProgress(float Progress);
	ACoreGameCharacter* GetCoreCharacter();
};
