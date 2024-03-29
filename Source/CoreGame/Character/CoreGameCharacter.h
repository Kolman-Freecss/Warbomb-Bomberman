// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreGame/RoundComponents/BombType.h"
#include "CoreGame/WarbombPrivateSystems/packages/KolmanFreecss/Core/Damageable/IDamageable.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CoreGameCharacter.generated.h"

class UCharacterInteractionInstigator;
class USphereComponent;
enum class BombType : uint8;
class UBaseLifeController;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ATCBomb;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerReciveBomb, int, Amount, BombType, Type);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerChangeWeaponBomb, BombType);


UCLASS(config = Game)
class ACoreGameCharacter : public ACharacter,
                           public IDamageable
{
	GENERATED_BODY()

#pragma region Components
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeBombFwdAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeBombBwdAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Instigator",
		meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractionArea = nullptr;

#pragma endregion

public:
	ACoreGameCharacter();

	FOnPlayerReciveBomb OnPlayerReciveBombEvent;
	FOnPlayerChangeWeaponBomb OnPlayerChangeWeaponBombEvent;

	UFUNCTION(BlueprintCallable)
	void AddBomb(int Quantity, BombType Type);

	virtual void TakeDamage(const float Damage, const AActor* _Instigator) override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = CoreCharacter)
	USoundBase* GetDamageSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = CoreCharacter)
	USoundBase* ThrowBombSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = CoreCharacter)
	USoundBase* NoBombSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = CoreCharacter)
	float BombDelayTime = 2.f;

	bool bCanThrowBomb = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = CoreCharacter)
	TMap<BombType, TSubclassOf<ATCBomb>> Bombs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = CoreCharacter)
	TMap<BombType, int> BombsPool;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = CoreCharacter)
	BombType CurrentBombType = BombType::WEAK;

	FTimerHandle BombTimerHandle;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGetDamage(const int Damage, const AActor* _Instigator);

	UFUNCTION(BlueprintImplementableEvent)
	FTransform BP_GetBombPosition();

	UFUNCTION(BlueprintCallable)
	void ThrowBomb();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ThrowBomb();

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Interaction();

	void ChangeBombFwd();
	
	void ChangeBombBwd();

public:
	UCharacterInteractionInstigator* InteractionInstigator;

	UFUNCTION(BlueprintCallable)
	void PlantBomb();

	void Fn3();

#pragma region Getters and Setters
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	int GetBombsQuantity() const;

	int GetBombsQuantity(BombType Type) const;

	BombType GetCurrentBombType() const;

	TMap<BombType, int> GetBombsPool() const;

	FORCEINLINE USphereComponent* GetInteractionArea() const { return InteractionArea; }
	
#pragma endregion
};
