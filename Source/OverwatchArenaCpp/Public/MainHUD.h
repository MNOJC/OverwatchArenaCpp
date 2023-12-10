// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCHARENACPP_API AMainHUD : public AHUD
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ScopeClass;

	UPROPERTY(VisibleInstanceOnly)
	class UScopeWidget* ScopeWidget;
	
	bool bScope = false;
	void ShowHideScope();
			
};
