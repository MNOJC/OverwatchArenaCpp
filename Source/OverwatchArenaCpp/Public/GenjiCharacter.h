// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Components/TimelineComponent.h"
#include "GenjiCharacter.generated.h"

class UTimelineComponent;

UCLASS()
class OVERWATCHARENACPP_API AGenjiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGenjiCharacter();
	
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* DashAction;

	// Dash implementation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash_Capacity)
	float DashLength = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline)
	UCurveFloat* DashingCurve;
	
	UPROPERTY()
	float PlayRate;
	
	
	UPROPERTY()
	FVector CurrentTimelineLocation;
	
	UPROPERTY()
	FVector TimelineStartLocation;
	
	UPROPERTY()
	FVector LocationToDash;
	
	UFUNCTION()
	void DashFinished();

	UFUNCTION()
	void DashUpdate(float Value);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	//Dash Functions
	void Dash();
	bool DashingLocation();
	UTimelineComponent*  DashTimeline;
	void BindDashTimeline();
	void NewPlayRateForDash();

	//Check ground
	bool CheckIfIsGrounded();
	
};
