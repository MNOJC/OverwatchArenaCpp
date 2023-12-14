// Fill out your copyright notice in the Description page of Project Settings.


#include "CharactersSelector.h"

#include "CharacterIcon.h"
#include "CustomCharacter.h"
#include "GenjiCharacter.h"
#include "Tracer.h"
#include "WidowmakerCharacter.h"
#include "Elements/Framework/TypedElementQueryBuilder.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACharactersSelector::ACharactersSelector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACharactersSelector::SwitchCharacterType(AActor* ActorOverlap)
{
	if (ActorOverlap->ActorHasTag("Player"))
	{
		const FVector ActorLocation = ActorOverlap->GetActorLocation();
		const FRotator ActorRotation = ActorOverlap->GetActorRotation();
		
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		
		switch (CharacterType)
		{
		case 0:
			//Do nothing
			break;

		case 1:
			
			PlayerController->Possess(GetWorld()->SpawnActor<AGenjiCharacter>(Genji, ActorLocation, ActorRotation));
			ActorOverlap->Destroy();
			break;

		
		case 2:
			PlayerController->Possess(GetWorld()->SpawnActor<AWidowmakerCharacter>(Widowmaker, ActorLocation, ActorRotation));
			ActorOverlap->Destroy();
			break;
			
		case 3:
			PlayerController->Possess(GetWorld()->SpawnActor<ATracer>(Tracer, ActorLocation, ActorRotation));
			ActorOverlap->Destroy();
			break;
			
		case 4:
			PlayerController->Possess(GetWorld()->SpawnActor<ACustomCharacter>(CustomCharacter, ActorLocation, ActorRotation));
			ActorOverlap->Destroy();
			break;
			
		}
		
		SetActorToLook();
		
	}
}

// Called when the game starts or when spawned
void ACharactersSelector::BeginPlay()
{
	Super::BeginPlay();
	
	if (const UWidgetComponent* WidgetComponent = FindComponentByClass<UWidgetComponent>())
	{
		UUserWidget* Widget = WidgetComponent->GetUserWidgetObject();
		
		if (UCharacterIcon* CharacterIconWidget = Cast<UCharacterIcon>(Widget))
			CharacterIconWidget->UpdateCharacterType(CharacterType);
		
		
	}

	SetActorToLook();
	
}

// Called every frame
void ACharactersSelector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UWidgetComponent* WidgetComponent = FindComponentByClass<UWidgetComponent>())
	{
		if (ActorToLook)
		{
		const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ActorToLook->GetActorLocation());
		const FRotator WidgetRotation = FRotator(0, LookRotation.Yaw, 0);
		WidgetComponent->SetRelativeRotation(WidgetRotation);
		}
	}

}

void ACharactersSelector::SetActorToLook()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Player", OutActors);

	for (AActor* CurrentActor : OutActors)
	{
		ActorToLook = CurrentActor;
		
	}
	
		TArray<AActor*> SelectorOutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_CharacterSelector, SelectorOutActors);

		for (AActor* CurrentActor : SelectorOutActors)
		{
			ACharactersSelector* CurrentCharactersSelector = Cast<ACharactersSelector>(CurrentActor);
			
			if (CurrentCharactersSelector)
			{
				CurrentCharactersSelector->ActorToLook = ActorToLook;
			}
			
		}
		
	
}

