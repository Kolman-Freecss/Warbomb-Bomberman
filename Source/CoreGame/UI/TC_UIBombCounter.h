// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TC_UIBombCounter.generated.h"

enum class BombType : uint8;
class UProgressBar;
class ACoreGameCharacter;

UCLASS()
class COREGAME_API UTC_UIBombCounter : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnReciveBomb(int Amount, bool bWithAnimation = true);

private:
	UFUNCTION()
	void OnReciveBomb(int Amount, BombType Type);
	ACoreGameCharacter* GetCoreCharacter() const;
};
