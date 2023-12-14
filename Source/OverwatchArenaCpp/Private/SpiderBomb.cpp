// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderBomb.h"

#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpiderBomb::ASpiderBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube->GetStaticMesh() = CubeMesh;


	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Cube);

}

// Called when the game starts or when spawned
void ASpiderBomb::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(CooldownExplosion, this, &ASpiderBomb::Explosion, CooldownBeforeExplosion, false,  CooldownBeforeExplosion);
}

// Called every frame
void ASpiderBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpiderBomb::SimulatePhysics()
{
	Cube->SetSimulatePhysics(false);
}

void ASpiderBomb::Explosion()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, Cube->GetComponentLocation());
	if (BP_Dummy)
	{
		BP_Dummy->PlayLightDamage();
	}
	Cube->DestroyComponent();
	GetWorld()->GetTimerManager().SetTimer(CooldownDestroyActor, this, &ASpiderBomb::DestroyActor, 0.5f, false,  0.5f);
}

void ASpiderBomb::DestroyActor()
{
	this->Destroy();
}

void ASpiderBomb::OverlapDummy(UObject* OtherActor)
{
	BP_Dummy = Cast<ADummy>(OtherActor);
}

