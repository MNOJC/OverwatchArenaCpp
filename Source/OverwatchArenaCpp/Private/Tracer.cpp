// Fill out your copyright notice in the Description page of Project Settings.


#include "Tracer.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "eDashOmni.h"
#include "StickyBomb.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATracer::ATracer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DashOmniTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	Camera->SetRelativeLocation(FVector(0, 0, 50));
}

// Called when the game starts or when spawned
void ATracer::BeginPlay()
{
	Super::BeginPlay();
	BindDashomniTimeline();
}

// Called every frame
void ATracer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATracer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ATracer::Move);
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Completed, this, &ATracer::SetDashToFrontDirection);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATracer::Look);


		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATracer::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATracer::StopJumping);

		EnhancedInputComponent->BindAction(OmniDash, ETriggerEvent::Triggered, this, &ATracer::Dash);

		EnhancedInputComponent->BindAction(StickyBomb, ETriggerEvent::Triggered, this, &ATracer::ThrowStickyBomb);

		
		
	}

}

void ATracer::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
		SetEnumerationDirection(MovementVector);
	}
}

void ATracer::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput((LookVector.X)  * - 1);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ATracer::Dash()
{
	if (!bDashing)
	{
		
		switch (DashOmnidirectionnal)
		{
		case 0:
				//None
			break;

		case 1:
			SetupDashLocation(GetActorForwardVector() * 750);
			break;

		case 2:
			
			SetupDashLocation(GetActorRightVector() * 750);
			break;

		case 3:
			
			SetupDashLocation(GetActorRightVector() * -1 * 750);
			break;

		case 4:
			
			SetupDashLocation(GetActorForwardVector() * -1 * 750);
			break;
		}

		bDashing = true;
		InitialActorLocation = GetActorLocation();
		DashOmniTimeline->PlayFromStart();
	}
	
	
}

void ATracer::SetupDashLocation(FVector Direction)
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = Direction + StartLocation;
	FHitResult HitResult;
		
	if (UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FColor::Red,
		FColor::Green))
	{
		LocationToDash = HitResult.Location;
	}
	else
	{
		LocationToDash = HitResult.TraceEnd;
	}

	
	
}

void ATracer::DashOmniFinished()
{
	bDashing = false;
}

void ATracer::DashOmniUpdate(float Value)
{
	
	this->SetActorLocation(UKismetMathLibrary::VLerp(InitialActorLocation, LocationToDash, Value));
}

void ATracer::BindDashomniTimeline()
{
	if(DashOmniCurve)
	{
		
		FOnTimelineFloat ProgressUpdate;
		ProgressUpdate.BindUFunction(this, FName("DashOmniUpdate"));
		DashOmniTimeline->AddInterpFloat(DashOmniCurve, ProgressUpdate);

		FOnTimelineEvent FinishedEvent;
		FinishedEvent.BindUFunction(this, FName("DashOmniFinished"));
		DashOmniTimeline->SetTimelineFinishedFunc(FinishedEvent);
		
		
	}
}

void ATracer::SetEnumerationDirection(FVector2d Value)
{
	if (Value.X > 0)
	{
		DashOmnidirectionnal = EDashOmni::Right;
	} else if (Value.X < 0)
	{
		DashOmnidirectionnal = EDashOmni::Left;
	}

	if (Value.Y > 0)
	{
		DashOmnidirectionnal = EDashOmni::Front;
	} else if (Value.Y < 0)
	{
		DashOmnidirectionnal = EDashOmni::Back;
	}
	
		
	
}

void ATracer::SetDashToFrontDirection()
{
	DashOmnidirectionnal = EDashOmni::Front;
}

void ATracer::ThrowStickyBomb()
{
	FVector Location = FVector (this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z + 20);
	
	FActorSpawnParameters SpawnParameters;
	AStickyBomb* StickyBombClass = GetWorld()->SpawnActor<AStickyBomb>(BP_StickyBomb, Location, this->GetActorRotation(), SpawnParameters);


	const UCameraComponent* CameraImpulse = GetComponentByClass<UCameraComponent>();
	if (CameraImpulse)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, "TEST");
		const FVector Direction =  CameraImpulse->GetForwardVector();
		const FVector Impulse = Direction * 800; 
		const FName BoneName = "None";
	
		StickyBombClass->Cube->AddImpulse(Impulse, BoneName, false);
	}
	
}



