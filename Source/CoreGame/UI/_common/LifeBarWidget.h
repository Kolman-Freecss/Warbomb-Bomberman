#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "LifeBarWidget.generated.h"

class UBaseLifeController;
class IDamageable;
class UProgressBar;
class ACoreGameCharacter;

UCLASS(Abstract, meta = (ShortTooltip = "Base class for life controllers"))
class COREGAME_API UTC_LifeBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* LifeBar = nullptr;

	UFUNCTION()
	void OnReciveDamage(float Damage, AActor* Instigator, UBaseLifeController* Target);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnReciveDamage(float Damage);
	void SetProgresBarProgress(float Progress) const;
};
