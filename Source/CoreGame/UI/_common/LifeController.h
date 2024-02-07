#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "LifeController.generated.h"

class UProgressBar;
class ACoreGameCharacter;

UCLASS(Abstract, meta = (ShortTooltip = "Base class for life controllers"))
class COREGAME_API UTC_LifeController : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UTC_LifeController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* LifeBar = nullptr;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnReciveDamage(float Damage);

	UFUNCTION()
	void OnReciveDamage(float Damage, float Percent);
	void SetProgresBarProgress(float Progress) const;
};
