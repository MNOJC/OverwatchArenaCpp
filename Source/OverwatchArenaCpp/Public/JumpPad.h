// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

UCLASS()
class OVERWATCHARENACPP_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpPad();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ImpulseForce = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Cube;

	UFUNCTION(BlueprintCallable)
	void ImpulseCharacter(AActor* ActorOverlap);
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
