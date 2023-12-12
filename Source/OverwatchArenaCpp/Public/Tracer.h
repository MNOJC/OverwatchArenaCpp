// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "StickyBomb.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "eDashOmni.h"
#include "Tracer.generated.h"

class UTimelineComponent;

UCLASS()
class OVERWATCHARENACPP_API ATracer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATracer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* OmniDash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* StickyBomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enumeration")
	EDashOmni DashOmnidirectionnal = EDashOmni::Front;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline)
	UCurveFloat* DashOmniCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpiderBomb)
	TSubclassOf<AStickyBomb> BP_StickyBomb;

	UFUNCTION()
	void DashOmniFinished();

	UFUNCTION()
	void DashOmniUpdate(float Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UTimelineComponent*  DashOmniTimeline;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dash();
	void SetupDashLocation(FVector EndLocation);
	void BindDashomniTimeline();
	void SetEnumerationDirection(FVector2d Value);
	void SetDashToFrontDirection();
	void ThrowStickyBomb();

	bool bDashing = false;
	FVector LocationToDash;
	FVector InitialActorLocation;

};
