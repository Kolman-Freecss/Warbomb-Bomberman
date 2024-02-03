// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TC_UIBombCounter.generated.h"

class UProgressBar;
class ACoreGameCharacter;

UCLASS()
class COREGAME_API UTC_UIBombCounter : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeOnInitialized();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnReciveBomb(int Amount, bool bWithAnimation = true);

private:

	UFUNCTION()
	void OnReciveBomb(int Amount);
	ACoreGameCharacter* GetCoreCharacter();
};
