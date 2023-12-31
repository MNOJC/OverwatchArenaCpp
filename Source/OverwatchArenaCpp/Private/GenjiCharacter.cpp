// Fill out your copyright notice in the Description page of Project Settings.

#include "GenjiCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// -------------------------------------------------------- CONSTRUCTOR  ------------------------------------------------------------------------------------------------- //
// Sets default values
AGenjiCharacter::AGenjiCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DashTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	
	TimelineStartLocation = FVector::ZeroVector; 
	LocationToDash = FVector::ZeroVector;

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
	
	BindDashTimeline();
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


		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AGenjiCharacter::Dash);

		EnhancedInputComponent->BindAction(WallClimbAction, ETriggerEvent::Triggered, this, &AGenjiCharacter::WallClimb);
		EnhancedInputComponent->BindAction(WallClimbAction, ETriggerEvent::Completed, this, &AGenjiCharacter::WallClimbFinished);
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

// -------------------------------------------------------- DASHING CAPACITY ------------------------------------------------------------------------------------------------- //
void AGenjiCharacter::Dash ()
{
	TimelineStartLocation = GetActorLocation();
	bool bDashing = true;
	if (DashingLocation())
	{
		NewPlayRateForDash();
	}
	
	DashTimeline->PlayFromStart();
		

	
}

bool AGenjiCharacter::DashingLocation()
{
	const UCameraComponent* Camera = GetComponentByClass<UCameraComponent>();
	const FRotator CameraRot = Camera->GetComponentRotation();
	const FVector CameraForwardVector = UKismetMathLibrary::GetForwardVector(CameraRot);
	const FVector DashVector = CameraForwardVector * DashLength; 

	
	const FVector StartLocation = Camera->GetComponentLocation();
	FVector EndLocation = DashVector + StartLocation;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 10, -1, 2);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params))
	{
		FVector HitLocation = HitResult.Location;
		LocationToDash = FVector(HitLocation.X, HitLocation.Y, (HitLocation.Z + 90));
		return true;
		
	} else {
		
		FVector TraceEnd = HitResult.TraceEnd;
		LocationToDash = FVector(TraceEnd.X, TraceEnd.Y, (TraceEnd.Z + 90));
		return false;
	}
	
}

void AGenjiCharacter::DashUpdate(const float Value)
{
	
	CurrentTimelineLocation = FMath::Lerp(TimelineStartLocation, LocationToDash, Value);
	
	SetActorLocation(CurrentTimelineLocation);
}

void AGenjiCharacter::DashFinished()
{
	
	if (!CheckIfIsGrounded())
	{
			const float OriginalGravityScale = GetCharacterMovement()-> GravityScale;
			GetCharacterMovement()->GravityScale = 0.2f;

			FTimerHandle TimerHandle;
		
		GetWorldTimerManager().SetTimer(TimerHandle, [this, OriginalGravityScale]()
		{
			if (GetCharacterMovement())
			{
		   
				GetCharacterMovement()->GravityScale = OriginalGravityScale;
			}
		}, 0.2f, false);
		
	}

	DashTimeline->SetPlayRate(1.0f);
}
void AGenjiCharacter::BindDashTimeline()
{
	if(DashingCurve)
	{
		
		FOnTimelineFloat ProgressUpdate;
		ProgressUpdate.BindUFunction(this, FName("DashUpdate"));
		DashTimeline->AddInterpFloat(DashingCurve, ProgressUpdate);

		FOnTimelineEvent FinishedEvent;
		FinishedEvent.BindUFunction(this, FName("DashFinished"));
		DashTimeline->SetTimelineFinishedFunc(FinishedEvent);
		
		
	}
}

void AGenjiCharacter::NewPlayRateForDash()
{
	const float Distance = UKismetMathLibrary::Vector_Distance(TimelineStartLocation, LocationToDash);
	PlayRate = ((1 - (Distance/DashLength)) + 1); 
	
	DashTimeline->SetPlayRate(PlayRate);
}

// -------------------------------------------------------- CHECK IF IS GROUNDED ------------------------------------------------------------------------------------------------- //

bool AGenjiCharacter::CheckIfIsGrounded()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;

	FVector StartLocation = GetActorLocation();
	FRotator ActorRotation = GetActorRotation();
	FVector ActorUpVector = UKismetMathLibrary::GetUpVector(ActorRotation);
	FVector EndLocation = (((ActorUpVector * -1) * 100) + StartLocation);
	
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params))
		return true;
	else
		return false;
}

// -------------------------------------------------------- WALL CLIMB ------------------------------------------------------------------------------------------------- //

bool AGenjiCharacter::CheckWallClimb()
{
	FVector StartLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector EndLocation = ForwardVector * 50 + StartLocation;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params))
		return true;
	else
		return false;
}

bool AGenjiCharacter::IsWalkingForward()
{
	if(GetVelocity().Y != 0)
		return true;
	else
		return false;
	
}

void AGenjiCharacter::WallClimb()
{
	if (CheckWallClimb() && IsWalkingForward() && bCanClimbing)
	{
		float WallClimbingDuration = 1.0f;
		
		Timer = Timer + UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		if (Timer > WallClimbingDuration)
		{
			Timer = WallClimbingDuration;
		}
		else
		{
			bIsCLimbing = true;
			
			LaunchCharacter(FVector(0, 0, 740), true, true);
		}
	} else {
		
		bIsCLimbing = false;
		Timer = 0;
	}
		
}

void AGenjiCharacter::WallClimbFinished()
{
	bIsCLimbing = false;
}


