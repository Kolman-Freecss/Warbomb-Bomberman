// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_common/LifeController.h"
#include "TC_UILifeBar.generated.h"

class UProgressBar;
class ACoreGameCharacter;

UCLASS()
class COREGAME_API UTC_UILifeBar : public UTC_LifeController
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	ACoreGameCharacter* GetCoreCharacter() const;
};
