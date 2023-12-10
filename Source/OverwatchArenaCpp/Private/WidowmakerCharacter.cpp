// Fill out your copyright notice in the Description page of Project Settings.


#include "WidowmakerCharacter.h"

#include "BlueprintEditor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CableComponent.h"
#include "MainHUD.h"
#include "SpiderBomb.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Materials/MaterialInterface.h"



// Sets default values
AWidowmakerCharacter::AWidowmakerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;
	GrappleHookTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	TargetDirectionBomb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetDirectionBomb"));
	TargetDirectionBomb->SetupAttachment( GetComponentByClass<UCameraComponent>());
	
}

// Called when the game starts or when spawned
void AWidowmakerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MainHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	
	BindGrappleHookTimeline();
	
}

// Called every frame
void AWidowmakerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// -------------------------------------------------------- INPUT ------------------------------------------------------------------------------------------------- //
// Called to bind functionality to input
void AWidowmakerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AWidowmakerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWidowmakerCharacter::Look);


		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWidowmakerCharacter::Jumping);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWidowmakerCharacter::StopJumping);

		EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Triggered, this, &AWidowmakerCharacter::GrappleHook);

		EnhancedInputComponent->BindAction(ScopeAction, ETriggerEvent::Started, this, &AWidowmakerCharacter::ShowScope);
		EnhancedInputComponent->BindAction(ScopeAction, ETriggerEvent::Canceled, this, &AWidowmakerCharacter::HideScope);
		EnhancedInputComponent->BindAction(ScopeAction, ETriggerEvent::Completed, this, &AWidowmakerCharacter::HideScope);

		EnhancedInputComponent->BindAction(BombAction, ETriggerEvent::Triggered, this, &AWidowmakerCharacter::ThrowBomb);
		
	}
}

void AWidowmakerCharacter::Jumping()
{
	ACharacter::Jump();
	if (bGrappleHookActivated)
	{
		ResetGrappleRender();
	}
}

void AWidowmakerCharacter::StopJumping()
{
	ACharacter::StopJumping();
}

void AWidowmakerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AWidowmakerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput((LookVector.X)  * - 1);
		AddControllerPitchInput(LookVector.Y);
	}
}

// -------------------------------------------------------- HOOK ------------------------------------------------------------------------------------------------- //

void AWidowmakerCharacter::GrappleHook()
{
	if (bGrappleHookActivated)
	{
		ResetGrappleRender();
	}
	else if (bCanGrappleHook)
	{
		bCanGrappleHook = false;
		bGrappleHookActivated = true;

		FVector StartLocation = GetActorLocation();
		const UCameraComponent* Camera = GetComponentByClass<UCameraComponent>();
			
		
		const FVector ForwardVector = Camera->GetForwardVector();
		FVector EndLocation = GrappleDistance * ForwardVector + GetActorLocation();
		
		
		FHitResult HitResult;

		
		if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLocation, EndLocation, 60.0f,  UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true, FColor::Green, FColor::Red))
		{
			if (!(UKismetMathLibrary::Vector_Distance(HitResult.Location, GetActorLocation()) < 300))
			{
				bBlockInput = true;
				GrappleHitLocation = HitResult.ImpactPoint;

				GrappleRender();	
				LaunchCharacter(FVector(0, 0, 500), true, true);
				
				
				GetWorld()->GetTimerManager().SetTimer(DelayedLaunchHandle, this, &AWidowmakerCharacter::DelayedFirstLaunch, 0.2f, false,  0.2f);

				
				GetWorld()->GetTimerManager().SetTimer(ResetRenderGrappleHandle, this, &AWidowmakerCharacter::ResetGrappleRender, 0.6f, false,  0.6f);
				
				
			}
		}
		else
		{
			GrappleHitLocation = HitResult.TraceEnd;
			GrappleRender();
			GetWorld()->GetTimerManager().SetTimer(ResetNoCollisionHandle, this, &AWidowmakerCharacter::ResetGrappleRender, 0.6f, false,  0.6f);
			
		}

		
	}
}

void AWidowmakerCharacter::GrappleRender()
{
	
	FName TemplateName = "Cable Component";
	const FTransform transform;
	
	UActorComponent* ActorComponent = this->AddComponentByClass(Cable, false, transform, false);
	CableComponent = Cast<UCableComponent>(ActorComponent);
	
	CableComponent->SetMaterial(0, MateriableCable);
	CableComponent->bAttachEnd = false;
	CableComponent->CableLength = 1000;
	CableComponent->CableWidth = 9;
	CableComponent->CableGravityScale = 0;

	CableComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	GrappleHookTimeline->PlayFromStart();
}

void AWidowmakerCharacter::GrappleHookUpdate(float Value)
{
	if (CableComponent)
	{
		FVector CableWorldLocation = UKismetMathLibrary::VInterpTo(CableComponent->GetComponentLocation(), GrappleHitLocation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), Value);
		CableComponent->SetWorldLocation(CableWorldLocation);
	}
	
}

void AWidowmakerCharacter::GrappleHookFinished()
{
	
}


void AWidowmakerCharacter::BindGrappleHookTimeline()
{
	if(GrappleHookCurve)
	{
		
		FOnTimelineFloat ProgressUpdate;
		ProgressUpdate.BindUFunction(this, FName("GrappleHookUpdate"));
		GrappleHookTimeline->AddInterpFloat(GrappleHookCurve, ProgressUpdate);

		FOnTimelineEvent FinishedEvent;
		FinishedEvent.BindUFunction(this, FName("GrappleHookFinished"));
		GrappleHookTimeline->SetTimelineFinishedFunc(FinishedEvent);
		
		
	}
}

void AWidowmakerCharacter::ResetGrappleRender()
{
	
	bGrappleHookActivated = false;
	bBlockInput = false;
	CableComponent->DestroyComponent();
	
	GetWorld()->GetTimerManager().SetTimer(CooldownHandle, this, &AWidowmakerCharacter::CanGrappleHook, GrappleCooldown, false,  GrappleCooldown);
	
	
		
	
}

void AWidowmakerCharacter::DelayedFirstLaunch()
{
	
	const FVector LaunchVector = FVector (FVector(GrappleHitLocation.X, GrappleHitLocation.Y, GrappleHitLocation.Z + 300) - GetActorLocation()); 
	LaunchCharacter(FVector(LaunchVector * 2.0f), true, true);
}


void AWidowmakerCharacter::CanGrappleHook()
{
	bCanGrappleHook = true;
}


// -------------------------------------------------------- SCOPE ------------------------------------------------------------------------------------------------- //

void AWidowmakerCharacter::ShowScope()
{
	if (UCameraComponent* Camera = GetComponentByClass<UCameraComponent>())
	{
		Camera->FieldOfView = 50.0f;
		MainHUD->ShowHideScope();
	}
}

void AWidowmakerCharacter::HideScope()
{
	if (UCameraComponent* Camera = GetComponentByClass<UCameraComponent>())
	{
		Camera->FieldOfView = 100.0f;
		MainHUD->ShowHideScope();
	}
}

// -------------------------------------------------------- SPIDER BOMB ------------------------------------------------------------------------------------------------- //

void AWidowmakerCharacter::ThrowBomb()
{
	FVector Location = FVector (this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z + 20);
	
	FActorSpawnParameters SpawnParameters;
	ASpiderBomb* SpiderBomb = GetWorld()->SpawnActor<ASpiderBomb>(BP_SpiderBomb, Location, this->GetActorRotation(), SpawnParameters);


	const UCameraComponent* Camera = GetComponentByClass<UCameraComponent>();
	if (Camera)
	{
		//FVector();
		const FVector Direction =  Camera->GetForwardVector();
		const FVector Impulse = Direction * 1500; 
		const FName BoneName = "None";
	
		SpiderBomb->Cube->AddImpulse(Impulse, BoneName, false);
	}
	
}
