// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BombType.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/HitResult.h"
#include "TCBomb.generated.h"

enum class BombType : uint8;
class UStaticMeshComponent;
class UParticleSystemComponent;
class USphereComponent;

UCLASS(/*meta = (DisableNativeTick)*/)
class COREGAME_API ATCBomb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATCBomb();

	// https://benui.ca/unreal/uproperty/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* Particle = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* DamageArea = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BombParams)
	float LifeTime = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BombParams)
	float ExplosionTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BombParams)
	float DamageRadius = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BombParams)
	int Damage = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BombParams)
	float InitialForce = 10000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BombParams)
	UParticleSystem* ExplosionParticles = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BombParams)
	BombType Type = BombType::WEAK;

	void OnExplode();
	void OnFinishExplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle ExplosionTimerHandle;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
