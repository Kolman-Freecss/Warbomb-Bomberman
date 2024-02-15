// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ObjectivesCounter.generated.h"

class ACoreGameGameMode;
class UProgressBar;
class ACoreGameCharacter;

UCLASS()
class COREGAME_API UObjectivesCounter : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextCounter;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnObjectiveCompleted(const bool bWithAnimation = true);

private:
	ACoreGameGameMode* GameMode;

	UFUNCTION()
	void OnObjectiveCompleted();

	FORCEINLINE void UpdateCounter(const int8 CurrentObjectivesCompleted, const int8 ObjectivesToComplete)
	{
		TextCounter->SetText(
			FText::FromString(
				FString::FromInt(CurrentObjectivesCompleted) + "/" + FString::FromInt(ObjectivesToComplete)));
	}
};
