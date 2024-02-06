// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CoreGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ATCBomb;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerReciveDamage, float, Damage, float, HealthPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerReciveBomb, int, Amount);


UCLASS(config = Game)
class ACoreGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	ACoreGameCharacter();

	FOnPlayerReciveDamage OnPlayerReciveDamageEvent;
	FOnPlayerReciveBomb OnPlayerReciveBombEvent;

	UFUNCTION(BlueprintCallable)
	void GetDamage(int Damage);

	UFUNCTION(BlueprintCallable)
	void AddBomb(int Quantity);
	int GetBombsQuantity() const;

protected:

	UPROPERTY(BlueprintReadOnly, category = CoreCharacter)
	int Life = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = CoreCharacter)
	int MaxLife = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = CoreCharacter)
	float BombDelayTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = CoreCharacter)
	int BombCounter = 2;

	bool bCanThrowBomb = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = CoreCharacter)
	TSubclassOf<ATCBomb> BombClass;

	FTimerHandle BombTimerHandle;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGetDamage(int Damage);

	UFUNCTION(BlueprintImplementableEvent)
	FTransform BP_GetBombPosition();


	UFUNCTION(BlueprintCallable)
	void ThrowBomb();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ThrowBomb();


	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void Fn3();
};