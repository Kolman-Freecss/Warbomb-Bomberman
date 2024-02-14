// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_common/LifeBarWidget.h"
#include "TC_UILifeBar.generated.h"

class UProgressBar;
class ACoreGameCharacter;

UCLASS()
class COREGAME_API UTC_UILifeBar : public UTC_LifeBarWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};
