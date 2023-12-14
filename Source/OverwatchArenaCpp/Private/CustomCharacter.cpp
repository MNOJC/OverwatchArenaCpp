// Fill out your copyright notice in the Description page of Project Settings.

#include "EnhancedInputComponent.h"
#include "CustomCharacter.h"

#include "ShockWave.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"


#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACustomCharacter::ACustomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	Camera->SetRelativeLocation(FVector(0, 0, 50));

}

// Called when the game starts or when spawned
void ACustomCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bSpeedActivated)
	{
		AddMovementInput(GetActorForwardVector(), 1);
	}
}

// Called to bind functionality to input
void ACustomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ACustomCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomCharacter::Look);


		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACustomCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACustomCharacter::StopJumping);

		EnhancedInputComponent->BindAction(CloneAction, ETriggerEvent::Triggered, this, &ACustomCharacter::CloneAbility);

		EnhancedInputComponent->BindAction(SpeedAction, ETriggerEvent::Triggered, this, &ACustomCharacter::ActivateSpeedAbility);

		EnhancedInputComponent->BindAction(GravityAction, ETriggerEvent::Triggered, this, &ACustomCharacter::GravityAbility);
		
	}
}

void ACustomCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr && !bSpeedActivated)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACustomCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput((LookVector.X)  * - 1);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ACustomCharacter::CloneAbility()
{
	if (bCloneActivated)
	{
		bCloneActivated= false;
		LastPlayerTransform = GetActorTransform();
		SetActorTransform(CloneTransform, false, nullptr);
		if (CloneRef)
		{
			CloneRef->SetActorTransform(LastPlayerTransform, false, nullptr);
			CloneRef->AutoDestroy();
		}
		
	}
	else
	{
		bCloneActivated = true;
		CloneTransform = GetActorTransform();
		const FActorSpawnParameters SpawnParameters;
		CloneRef = GetWorld()->SpawnActor<AClone>(BP_Clone, GetActorLocation(), GetActorRotation(), SpawnParameters);
		
	
	}
}

void ACustomCharacter::ActivateSpeedAbility()
{
	GetCharacterMovement()->MaxWalkSpeed = 2500;
	Camera->SetFieldOfView(150);
	bSpeedActivated = true;
	GetWorld()->GetTimerManager().SetTimer(SpeedLenght, this, &ACustomCharacter::DeactivateSpeedAbility, 3.0f, false,  3.0f);
}

void ACustomCharacter::DeactivateSpeedAbility()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;
	Camera->SetFieldOfView(100);
	bSpeedActivated = false;
}

void ACustomCharacter::GravityAbility()
{
	if (bCanGravity)
	{
		bCanGravity = false;
		if (BP_ShockWave)
		{
			
			const FActorSpawnParameters Parameters;
			BP_ShockWaveRef = GetWorld()->SpawnActor<AShockWave>(BP_ShockWave, GetActorTransform(), Parameters);
			BP_ShockWaveRef->Cube->AddImpulse(Camera->GetForwardVector() * 1000, NAME_None, false);
			GetWorld()->GetTimerManager().SetTimer(GravityHandle, this, &ACustomCharacter::ResetGravityCapacity, 4.0f, false,  4.0f);
		}
		
	}
}

void ACustomCharacter::ResetGravityCapacity()
{
	bCanGravity = true;
}
