// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dummy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SpiderBomb.generated.h"

UCLASS()
class OVERWATCHARENACPP_API ASpiderBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpiderBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= StaticMesh)
	UStaticMeshComponent* Cube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= StaticMesh)
	UStaticMesh* CubeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Sphere)
	USphereComponent* Sphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= StaticMesh)
	UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= StaticMesh)
	ADummy* BP_Dummy;

	UFUNCTION(BlueprintCallable)
	void SimulatePhysics();

	UFUNCTION(BlueprintCallable)
	void OverlapDummy(UObject* OtherActor);

	
	void Explosion();
	void DestroyActor();

	float CooldownBeforeExplosion = 3.0f;
	FTimerHandle CooldownExplosion;
	FTimerHandle CooldownDestroyActor;

};
