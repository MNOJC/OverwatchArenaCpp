// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dummy.generated.h"

UCLASS()
class OVERWATCHARENACPP_API ADummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADummy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= SkeletalMesh)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= SkeletalMesh)
	UStaticMeshComponent* TargetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Animations)
	UAnimationAsset* HardHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Animations)
	UAnimationAsset* LightHit;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayLightDamage();
	void PlayHardDamage();

};


