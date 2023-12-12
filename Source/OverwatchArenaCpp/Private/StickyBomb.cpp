// Fill out your copyright notice in the Description page of Project Settings.


#include "StickyBomb.h"

#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AStickyBomb::AStickyBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BombTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube->GetStaticMesh() = CubeMesh;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereCollision->SetupAttachment(Cube);
	SphereCollision->SetSphereRadius(200);
}

// Called when the game starts or when spawned
void AStickyBomb::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(CooldownExplosion, this, &AStickyBomb::Explosion, CooldownBeforeExplosion, false,  CooldownBeforeExplosion);
	BindBombTimeline();

}

// Called every frame
void AStickyBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStickyBomb::SimulatePhysics()
{
	Cube->SetSimulatePhysics(false);
}

void AStickyBomb::StickToDummy(UObject* Object)
{
	BP_Dummy = Cast<ADummy>(Object);
	BombTimeline->PlayFromStart();
}

void AStickyBomb::Explosion()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, Cube->GetComponentLocation());
	if (BP_Dummy)
	{
		BP_Dummy->PlayHardDamage();
	}
	Cube->DestroyComponent();
	GetWorld()->GetTimerManager().SetTimer(CooldownDestroyActor, this, &AStickyBomb::DestroyActor, 0.5f, false,  0.5f);
}

void AStickyBomb::DestroyActor()
{
	this->Destroy();
}

void AStickyBomb::BombFinished()
{
	if (BP_Dummy)
	{
		Cube->SetSimulatePhysics(false);
		Cube->AttachToComponent(BP_Dummy->TargetMesh, FAttachmentTransformRules::KeepWorldTransform);
	}
	
}

void AStickyBomb::BombUpdate(float Value)
{
	if (IsValid(Cube))
	{
		if (BP_Dummy)
		{
			const FVector Location = UKismetMathLibrary::VLerp(Cube->GetComponentLocation(), BP_Dummy->TargetMesh->GetComponentLocation(), Value);
			Cube->SetWorldLocation(Location, false);
		}
		
	}
}

void AStickyBomb::BindBombTimeline()
{
	if(BombCurve)
	{
		
		FOnTimelineFloat ProgressUpdate;
		ProgressUpdate.BindUFunction(this, FName("BombUpdate"));
		BombTimeline->AddInterpFloat(BombCurve, ProgressUpdate);

		FOnTimelineEvent FinishedEvent;
		FinishedEvent.BindUFunction(this, FName("BombFinished"));
		BombTimeline->SetTimelineFinishedFunc(FinishedEvent);
		
		
	}
}

