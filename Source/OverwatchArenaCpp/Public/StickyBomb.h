// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dummy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "StickyBomb.generated.h"

class UTimelineComponent;

UCLASS()
class OVERWATCHARENACPP_API AStickyBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStickyBomb();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= StaticMesh)
	UStaticMeshComponent* Cube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= StaticMesh)
	USphereComponent* SphereCollision;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= StaticMesh)
	UStaticMesh* CubeMesh;
    	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= StaticMesh)
	UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline)
	UCurveFloat* BombCurve;

	UFUNCTION(BlueprintCallable)
	void SimulatePhysics();

	UFUNCTION(BlueprintCallable)
	void StickToDummy(UObject* Object);

	UFUNCTION()
	void BombFinished();

	UFUNCTION()
	void BombUpdate(float Value);

	
	void Explosion();
	void DestroyActor();

	float CooldownBeforeExplosion = 3.0f;
	FTimerHandle CooldownExplosion;
	FTimerHandle CooldownDestroyActor;

	ADummy* BP_Dummy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UTimelineComponent*  BombTimeline;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BindBombTimeline();

};
