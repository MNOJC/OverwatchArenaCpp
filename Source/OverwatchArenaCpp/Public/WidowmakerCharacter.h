// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CableComponent.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Components/TimelineComponent.h"
#include "WidowmakerCharacter.generated.h"

class UTimelineComponent;
class UCableComponent;

UCLASS()
class OVERWATCHARENACPP_API AWidowmakerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWidowmakerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* JumpAction;

	//Grapple hook
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* GrappleAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material)
	UMaterialInterface* MateriableCable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material)
	TSubclassOf<class UCableComponent> Cable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameters)
	float GrappleCooldown = 1.0f;

	//Timeline

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline)
	UCurveFloat* GrappleHookCurve;

	UFUNCTION()
	void GrappleHookFinished();

	UFUNCTION()
	void GrappleHookUpdate(float Value);

	//Widowmaker parameters

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Parameters)
	float GrappleDistance = 1900.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UTimelineComponent*  GrappleHookTimeline;
	

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void GrappleHook();
	void Jumping();
	void StopJumping();
	void GrappleRender();
	void BindGrappleHookTimeline();
	void ResetGrappleRender();
	void CanGrappleHook();

	void DelayedFirstLaunch();

	bool bGrappleHookActivated = false;
	bool bCanGrappleHook = true;
	bool bBlockInput = false;
	
	FTimerHandle DelayedLaunchHandle;
	FTimerHandle ResetRenderGrappleHandle;
	FTimerHandle CooldownHandle;
	FTimerHandle ResetNoCollisionHandle;
	

	FVector GrappleHitLocation;
	UCableComponent* CableComponent;

};
