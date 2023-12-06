// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "eCharacterType.generated.h"


UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	None		UMETA(DisplayName = "No character type"),
	Genji		UMETA(DisplayName = "Genji character"),
	Widowmaker	UMETA(DisplayName = "Widowmaker character"),
	Tracer		UMETA(DisplayName = "Tracer character"),
	Custom		UMETA(DisplayName = "Custom character"),
	
};

