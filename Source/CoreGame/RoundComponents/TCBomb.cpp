// Fill out your copyright notice in the Description page of Project Settings.


#include "TCBomb.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreGame/Character/CoreGameCharacter.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATCBomb::ATCBomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	Particle->SetupAttachment(Mesh);
	Particle->bAutoActivate = true;

	DamageArea = CreateDefaultSubobject<USphereComponent>(TEXT("DamageArea"));
	DamageArea->SetupAttachment(Mesh);
	DamageArea->SetSphereRadius(0.f);
}

// Called when the game starts or when spawned
void ATCBomb::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ATCBomb::OnExplode, LifeTime, false);
	const FVector ImpulseDirection = Mesh->GetForwardVector() * InitialForce;
	Mesh->AddImpulse(ImpulseDirection);
}

void ATCBomb::OnExplode()
{
	DamageArea->OnComponentBeginOverlap.AddUniqueDynamic(this, &ATCBomb::OnBeginOverlap);
	Particle->SetTemplate(ExplosionParticles);
	Particle->Activate();
	Mesh->SetVisibility(false);
	Mesh->SetSimulatePhysics(false);

	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ATCBomb::OnFinishExplosion, ExplosionTime, false);
	DamageArea->SetSphereRadius(DamageRadius);
}

void ATCBomb::OnFinishExplosion()
{
	ExplosionTimerHandle.Invalidate();
	this->Destroy();
}

void ATCBomb::OnBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IDamageable* Damageable = Cast<IDamageable>(OtherActor))
	{
		Damageable->TakeDamage(Damage, this);
	}
}
