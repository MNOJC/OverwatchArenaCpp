// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



UENUM(BlueprintType)
enum class EDashOmni : uint8
{
	None		UMETA(DisplayName = "No character type"),
	Front		UMETA(DisplayName = "Front direction"),
	Right		UMETA(DisplayName = "Right direction"),
	Left		UMETA(DisplayName = "Left direction"),
	Back		UMETA(DisplayName = "Back direction"),
	
};

