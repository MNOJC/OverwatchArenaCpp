// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"

// Sets default values
ADummy::ADummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
	TargetMesh->SetupAttachment(SkeletalMesh);
	TargetMesh->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "Head");
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADummy::PlayLightDamage()
{
	SkeletalMesh->PlayAnimation(LightHit, false);
}

void ADummy::PlayHardDamage()
{
	SkeletalMesh->PlayAnimation(HardHit, false);
}

