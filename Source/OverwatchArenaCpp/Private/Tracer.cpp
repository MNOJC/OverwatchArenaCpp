// Fill out your copyright notice in the Description page of Project Settings.


#include "Tracer.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "eDashOmni.h"
#include "StickyBomb.h"
#include "CollisionQueryParams.h"
#include "Kismet/GameplayStatics.h"
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

	TickCounter = 0;
	TickInterval = 0.05;
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

	if (bRecallPressed)
	{
		Recall();
	}
	TickCounter++;
	if (TickCounter >= FMath::RoundToInt(1.0f / DeltaTime * TickInterval))
		{
			if (!bRecallPressed)
			{
				Recall();
				TickCounter = 0;
			}
		
		}
	
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

		EnhancedInputComponent->BindAction(RecallAction, ETriggerEvent::Triggered, this, &ATracer::RecallInput);

		
		
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
		
		const FVector Direction =  CameraImpulse->GetForwardVector();
		const FVector Impulse = Direction * 800; 
		const FName BoneName = "None";
	
		StickyBombClass->Cube->AddImpulse(Impulse, BoneName, false);
	}
	
}

void ATracer::RecallInput()
{
	if (bCanRecall)
	{
		bRecallPressed= true;
		bCanRecall = false;
		bCanSetPos = false;
		Counter = RecallPositions.Num() - 1;
	}
}

void ATracer::Recall()
{
	
	if (bCanSetPos)
	{
		//GetWorld()->GetTimerManager().SetTimer(DelayPosHandle, this, &ATracer::SetRecallNewPos, TimeBeforeSettingNewPosition, false,  TimeBeforeSettingNewPosition);
		SetRecallNewPos();
		
	}

	if (bRecallPressed)
	{
		const float AlphaTravelSpeed = UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * TravelSpeed;
		if (Counter >= 0 && Counter < RecallPositions.Num())
		{
			
			ActorLastTransform = UKismetMathLibrary::TLerp(GetActorTransform(), RecallPositions[Counter], AlphaTravelSpeed);
		}
		else
		{
			
		}
		this->SetActorLocation(ActorLastTransform.GetLocation());

		const float AlphaRotationSpeed = UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * RotationSpeed;
		const FRotator ActorRotation = UKismetMathLibrary::RLerp(GetActorRotation(), ActorLastTransform.Rotator(), AlphaRotationSpeed, false);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(ActorRotation);

		if (Counter >= 0 && Counter < RecallPositions.Num())
		{
			if (UKismetMathLibrary::NearlyEqual_TransformTransform(GetActorTransform(), RecallPositions[Counter]), 300, 300, 300)
			{
				if (Counter == 0)
				{
					bCanSetPos = true;
					bRecallPressed = false;
					RecallPositions.Empty();
					GetWorld()->GetTimerManager().SetTimer(CooldownRecallHandle, this, &ATracer::ResetRecallCapacity, 2.0f, false,  2.0f);
				
				}
				else
				{
					Counter = Counter -1;
				}
			}
		}
		
	}
}

void ATracer::SetRecallNewPos()
{
	if (RecallPositions.Num() >= 150)
	{
		RecallPositions.RemoveAt(0);
	}
	else
	{
		
		const int Index = RecallPositions.Add(GetActorTransform());
		const FTransform Frustum = RecallPositions[Index];
		const FMatrix FrustumMatrix = Frustum.ToMatrixWithScale();
		//DrawDebugFrustum(GetWorld(), FrustumMatrix, FColor::Red, false, 6.0f, 0.0f, 15.0f);
	}
}

void ATracer::ResetRecallCapacity()
{
	bCanRecall = true;
}


