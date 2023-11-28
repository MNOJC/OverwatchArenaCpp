// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

#include "GenjiCharacter.h"
#include "MainPlayerController.h"

AMainGameMode::AMainGameMode()
{
	DefaultPawnClass = AGenjiCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
}
