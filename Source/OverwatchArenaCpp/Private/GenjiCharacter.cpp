// Fill out your copyright notice in the Description page of Project Settings.

#include "GenjiCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// -------------------------------------------------------- CONSTRUCTOR  ------------------------------------------------------------------------------------------------- //
// Sets default values
AGenjiCharacter::AGenjiCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// -------------------------------------------------------- BEGIN PLAY  ------------------------------------------------------------------------------------------------- //
// Called when the game starts or when spawned
void AGenjiCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	} 
	
}


// -------------------------------------------------------- TICK  ------------------------------------------------------------------------------------------------- //
// Called every frame
void AGenjiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// -------------------------------------------------------- INPUT ------------------------------------------------------------------------------------------------- //
void AGenjiCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AGenjiCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGenjiCharacter::Look);
	}
}

void AGenjiCharacter::Move(const FInputActionValue& Value)
{
	 const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGenjiCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput((LookVector.X)  * - 1);
		AddControllerPitchInput(LookVector.Y);
	}
}

