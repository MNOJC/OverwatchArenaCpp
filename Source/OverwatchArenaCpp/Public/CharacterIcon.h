// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "eCharacterType.h"
#include "Blueprint/UserWidget.h"
#include "CharacterIcon.generated.h"

UCLASS()
class OVERWATCHARENACPP_API UCharacterIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enumeration")
	ECharacterType CharacterType = ECharacterType::Genji;
	
	void UpdateCharacterType (ECharacterType Type);
};
