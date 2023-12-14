// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Clone.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "ShockWave.h"
#include "CustomCharacter.generated.h"

UCLASS()
class OVERWATCHARENACPP_API ACustomCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* CloneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SpeedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* GravityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ref)
	TSubclassOf<AClone> BP_Clone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ref)
	TSubclassOf<AShockWave> BP_ShockWave;
 
	//Components

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UCameraComponent* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	//Abilities

	void CloneAbility();
	void ActivateSpeedAbility();
	void DeactivateSpeedAbility();
	void GravityAbility();
	void ResetGravityCapacity();
	

	FTransform CloneTransform;
	bool bCloneActivated;
	AClone* CloneRef;
	AShockWave* BP_ShockWaveRef;
	FTransform LastPlayerTransform;
	bool bSpeedActivated;
	FTimerHandle SpeedLenght;
	FTimerHandle GravityHandle;
	bool bCanGravity = true;

};
