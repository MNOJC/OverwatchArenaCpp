// Fill out your copyright notice in the Description page of Project Settings.


#include "Clone.h"

#include "Chaos/GeometryParticlesfwd.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AClone::AClone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(SkeletalMeshComponent);

}

// Called when the game starts or when spawned
void AClone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClone::AutoDestroy()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation(), FRotator::ZeroRotator, FVector(5), true, EPSCPoolMethod::None, true);
	SkeletalMeshComponent->DestroyComponent();
	if (IsValid(BP_Dummy))
	{
		BP_Dummy->PlayHardDamage();
	}
	GetWorld()->GetTimerManager().SetTimer(CooldownDestroyActor, this, &AClone::DestroyActor, 0.5f, false, 0.5f);
}

void AClone::OverlapDummy(UObject* OtherActor)
{
	BP_Dummy = Cast<ADummy>(OtherActor);
}

void AClone::DestroyActor()
{
	this->Destroy();
}

