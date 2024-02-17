// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "../RoundComponents/TCBomb.h"
#include "Components/SphereComponent.h"
#include "CoreGame/WarbombPrivateSystems/packages/KolmanFreecss/Config/GameInstance/CoreGameInstance.h"
#include "CoreGame/WarbombPrivateSystems/packages/KolmanFreecss/Core/Character/UCharacterInteractionInstigator.h"
#include "CoreGame/WarbombPrivateSystems/packages/KolmanFreecss/Core/_common/BaseLifeController.h"
#include "PhysicsEngine/BodySetup.h"
#include "PhysicsEngine/ConstraintInstance.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACoreGameCharacter

ACoreGameCharacter::ACoreGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	InteractionArea = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionArea"));
	InteractionArea->SetupAttachment(RootComponent);

	UE_LOG(LogTemplateCharacter, Warning, TEXT("ACoreGameCharacter Constructor"));

	if (BombsPool.Num() == 0)
	{
		BombsPool.Add(BombType::WEAK, 2);
		BombsPool.Add(BombType::STRONG, 0);
	}

	CurrentBombType = BombType::WEAK;
}

void ACoreGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACoreGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACoreGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACoreGameCharacter::Look);

		EnhancedInputComponent->BindAction(ChangeBombFwdAction, ETriggerEvent::Triggered, this,
		                                   &ACoreGameCharacter::ChangeBombFwd);

		EnhancedInputComponent->BindAction(ChangeBombBwdAction, ETriggerEvent::Triggered, this,
		                                   &ACoreGameCharacter::ChangeBombBwd);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}

	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAction("ThrowBomb", IE_Pressed, this, &ACoreGameCharacter::ThrowBomb);
		PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &ACoreGameCharacter::Interaction);
		PlayerInputComponent->BindAction("PlantBomb", IE_Pressed, this, &ACoreGameCharacter::PlantBomb);
	}
}

void ACoreGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACoreGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACoreGameCharacter::ChangeBombFwd()
{
	uint8 NewBombType = static_cast<uint8>(CurrentBombType) + 1;
	if (NewBombType >= static_cast<uint8>(BombType::MAX))
	{
		NewBombType = 0;
	}
	CurrentBombType = static_cast<BombType>(NewBombType);
	OnPlayerChangeWeaponBombEvent.Broadcast(CurrentBombType);
}

void ACoreGameCharacter::ChangeBombBwd()
{
	uint8 NewBombType = static_cast<uint8>(CurrentBombType) - 1;
	if (NewBombType >= static_cast<uint8>(BombType::MAX))
	{
		NewBombType = static_cast<uint8>(BombType::MAX) - 1;
	}
	CurrentBombType = static_cast<BombType>(NewBombType);
	OnPlayerChangeWeaponBombEvent.Broadcast(CurrentBombType);
}

void ACoreGameCharacter::TakeDamage(const float Damage, const AActor* _Instigator)
{
	if (GetDamageSound)
		Cast<UCoreGameInstance>(GetGameInstance())->Play2DSFXCommonSound(GetDamageSound);
	else
		UE_LOG(LogTemplateCharacter, Warning, TEXT("GetDamageSound is not set"));
	OnTakeDamage.Broadcast(Damage, _Instigator, nullptr);
	BP_OnGetDamage(Damage, _Instigator);
}

void ACoreGameCharacter::ThrowBomb()
{
	if (!bCanThrowBomb || BombsPool[CurrentBombType] <= 0)
	{
		if (NoBombSound)
			Cast<UCoreGameInstance>(GetGameInstance())->Play2DSFXCommonSound(NoBombSound);
		else
			UE_LOG(LogTemplateCharacter, Warning, TEXT("NoBombSound is not set"));
		return;
	}

	// Spawn Bomb C++
	const FTransform BombPositon = BP_GetBombPosition();
	TSubclassOf<ATCBomb>* SubclassPtr = &(Bombs[CurrentBombType]);
	ATCBomb* Bomb = GetWorld()->SpawnActor<ATCBomb>((*SubclassPtr), BombPositon);

	if (ThrowBombSound)
		Cast<UCoreGameInstance>(GetGameInstance())->Play2DSFXCommonSound(ThrowBombSound);
	else
		UE_LOG(LogTemplateCharacter, Warning, TEXT("ThrowBombSound is not set"));
	
	// Set timer: Bomb delay
	bCanThrowBomb = false;
	BombsPool[CurrentBombType]--;
	GetWorldTimerManager().SetTimer(BombTimerHandle, this, &ACoreGameCharacter::Fn3, BombDelayTime, false);

	// Call the event to update the UI.
	OnPlayerReciveBombEvent.Broadcast(BombsPool[CurrentBombType], CurrentBombType);
}

void ACoreGameCharacter::PlantBomb()
{
	if (BombsPool[CurrentBombType] <= 0)
	{
		if (NoBombSound)
			Cast<UCoreGameInstance>(GetGameInstance())->Play2DSFXCommonSound(NoBombSound);
		else
			UE_LOG(LogTemplateCharacter, Warning, TEXT("NoBombSound is not set"));
		return;
	}
	
	// Spawn Bomb C++
	const FTransform BombPositon = BP_GetBombPosition();
	TSubclassOf<ATCBomb>* SubclassPtr = &(Bombs[CurrentBombType]);
	ATCBomb* Bomb = GetWorld()->SpawnActor<ATCBomb>((*SubclassPtr), BombPositon);
	// Freeze position.
	Bomb->Mesh->GetBodyInstance()->bLockXRotation = true;
	Bomb->Mesh->GetBodyInstance()->bLockYRotation = true;
	Bomb->Mesh->GetBodyInstance()->bLockZRotation = true;
	Bomb->Mesh->GetBodyInstance()->bLockXTranslation = true;
	Bomb->Mesh->GetBodyInstance()->bLockYTranslation = true;
	Bomb->Mesh->GetBodyInstance()->SetDOFLock(EDOFMode::SixDOF);
	

	if (ThrowBombSound)
		Cast<UCoreGameInstance>(GetGameInstance())->Play2DSFXCommonSound(ThrowBombSound);
	else
		UE_LOG(LogTemplateCharacter, Warning, TEXT("ThrowBombSound is not set"));

	BombsPool[CurrentBombType]--;
	GetWorldTimerManager().SetTimer(BombTimerHandle, this, &ACoreGameCharacter::Fn3, BombDelayTime, false);

	// Call the event to update the UI.
	OnPlayerReciveBombEvent.Broadcast(BombsPool[CurrentBombType], CurrentBombType);
}

void ACoreGameCharacter::Interaction()
{
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Interaction"));
	InteractionInstigator->OnInteraction();
}

void ACoreGameCharacter::AddBomb(int Quantity, BombType Type)
{
	BombsPool[Type] += Quantity;

	// Call the event to update the UI.
	OnPlayerReciveBombEvent.Broadcast(BombsPool[Type], Type);
}

void ACoreGameCharacter::Fn3()
{
	bCanThrowBomb = true;
}

#pragma region Getter and Setter

/**
 * Get the quantity of bombs of current type.
 * @return
 */
int ACoreGameCharacter::GetBombsQuantity() const
{
	return BombsPool[CurrentBombType];
}

/**
 * Get the quantity of bombs of a specific type.
 * @param Type
 * @return
 */
int ACoreGameCharacter::GetBombsQuantity(BombType Type) const
{
	return BombsPool[Type];
}


TMap<BombType, int> ACoreGameCharacter::GetBombsPool() const
{
	return BombsPool;
}

BombType ACoreGameCharacter::GetCurrentBombType() const
{
	return CurrentBombType;
}

#pragma endregion
