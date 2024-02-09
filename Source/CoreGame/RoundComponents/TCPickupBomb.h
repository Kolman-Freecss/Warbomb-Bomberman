// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BombType.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/HitResult.h"
#include "TCPickupBomb.generated.h"

enum class BombType : uint8;
class UStaticMeshComponent;
class UParticleSystemComponent;
class USphereComponent;

UCLASS(/*meta = (DisableNativeTick)*/)
class COREGAME_API ATCPickupBomb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATCPickupBomb();

	// https://benui.ca/unreal/uproperty/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* Particle = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* DamageArea = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	BombType BombType = BombType::WEAK;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = CoreCharacter)
	int Amount = 1;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
