// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPad.h"
#include "GameFramework/Character.h"

// Sets default values
AJumpPad::AJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	

}

// Called when the game starts or when spawned
void AJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJumpPad::ImpulseCharacter(AActor* ActorOverlap)
{
	if (ActorOverlap->ActorHasTag("Player"))
	{
		ACharacter* CharacterOverlap = Cast<ACharacter>(ActorOverlap);

		if (CharacterOverlap != nullptr)
		{
			CharacterOverlap->LaunchCharacter(Cube->GetUpVector() * ImpulseForce, true, true);
		}	
	}
	
}