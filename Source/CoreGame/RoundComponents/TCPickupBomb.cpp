// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPickupBomb.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreGame/Character/CoreGameCharacter.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATCPickupBomb::ATCPickupBomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(false);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	Particle->SetupAttachment(Mesh);
	Particle->bAutoActivate = true;

	DamageArea = CreateDefaultSubobject<USphereComponent>(TEXT("DamageArea"));
	DamageArea->SetupAttachment(Mesh);
	DamageArea->SetSphereRadius(0.f);
}

// Called when the game starts or when spawned
void ATCPickupBomb::BeginPlay()
{
	Super::BeginPlay();
	DamageArea->OnComponentBeginOverlap.AddUniqueDynamic(this, &ATCPickupBomb::OnBeginOverlap);
}

void ATCPickupBomb::OnBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	if (ACoreGameCharacter* MyCharacter = Cast<ACoreGameCharacter>(OtherActor))
	{
		Particle->Activate();
		MyCharacter->AddBomb(Amount, BombType);

		//Mesh->SetVisibility(false);
		//Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Destroy();
	}
}
