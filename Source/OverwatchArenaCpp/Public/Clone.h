// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dummy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Clone.generated.h"

UCLASS()
class OVERWATCHARENACPP_API AClone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClone();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* Particle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADummy* BP_Dummy;

	UFUNCTION(BlueprintCallable)
	void OverlapDummy (UObject* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void DestroyActor();
	void AutoDestroy();
	
	FTimerHandle CooldownDestroyActor;
	

};
