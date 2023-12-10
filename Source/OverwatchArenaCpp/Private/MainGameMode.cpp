// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "MainHUD.h"
#include "MainPlayerController.h"



AMainGameMode::AMainGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Characters/Genji/BP_GenjiCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	PlayerControllerClass = AMainPlayerController::StaticClass();
	
}
