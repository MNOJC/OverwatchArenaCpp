// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ShockWave.generated.h"

UCLASS()
class OVERWATCHARENACPP_API AShockWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShockWave();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Cube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* Sphere;

	UFUNCTION(BlueprintCallable)
	void ImpulseDummy(UObject* OtherActor);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroyActor();
	FTimerHandle CooldownBeforeDestroy;

};
