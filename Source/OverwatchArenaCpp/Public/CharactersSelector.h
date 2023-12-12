// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "eCharacterType.h"
#include "Components/WidgetComponent.h"
#include "CharactersSelector.generated.h"



UCLASS()
class OVERWATCHARENACPP_API ACharactersSelector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharactersSelector();

	UFUNCTION(BlueprintCallable)
	void SwitchCharacterType (AActor* ActorOverlap);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Characters")
	TSubclassOf<class AGenjiCharacter> Genji;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Characters")
	TSubclassOf<class AWidowmakerCharacter> Widowmaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Characters")
	TSubclassOf<class ATracer> Tracer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enumeration")
	ECharacterType CharacterType = ECharacterType::Genji;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	AActor* ActorToLook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Class ref")
	TSubclassOf<class ACharactersSelector> BP_CharacterSelector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetActorToLook ();

};


