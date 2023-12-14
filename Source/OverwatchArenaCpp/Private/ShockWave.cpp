// Fill out your copyright notice in the Description page of Project Settings.


#include "ShockWave.h"

#include "Dummy.h"

// Sets default values
AShockWave::AShockWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube->SetupAttachment(RootComponent);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Cube);

}

// Called when the game starts or when spawned
void AShockWave::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShockWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShockWave::ImpulseDummy(UObject* OtherActor)
{
	ADummy* BP_Dummy = Cast<ADummy>(OtherActor);
	if (BP_Dummy)
	{
		BP_Dummy->Impulse();
	}
	
	GetWorld()->GetTimerManager().SetTimer(CooldownBeforeDestroy, this, &AShockWave::DestroyActor, 2.0f, false,  2.0f);
}

void AShockWave::DestroyActor()
{
	this->Destroy();
}
