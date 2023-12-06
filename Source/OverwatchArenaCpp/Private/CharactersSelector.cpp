// Fill out your copyright notice in the Description page of Project Settings.


#include "CharactersSelector.h"

#include "CharacterIcon.h"
#include "GenjiCharacter.h"
#include "WidowmakerCharacter.h"
#include "Elements/Framework/TypedElementQueryBuilder.h"
#include "Kismet/GameplayStatics.h"

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

			break;
			
		case 4:

			break;
			
		}

		
		
	}
}

// Called when the game starts or when spawned
void ACharactersSelector::BeginPlay()
{
	Super::BeginPlay();

	UWidgetComponent* WidgetComponent = FindComponentByClass<UWidgetComponent>();
	
	if (WidgetComponent)
	{
		UUserWidget* Widget = WidgetComponent->GetUserWidgetObject();
		UCharacterIcon* CharacterIconWidget = Cast<UCharacterIcon>(Widget);

		if (CharacterIconWidget)
			CharacterIconWidget->UpdateCharacterType(CharacterType);
		
		
	}
	
}

// Called every frame
void ACharactersSelector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

