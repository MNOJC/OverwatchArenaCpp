// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

#include "ScopeWidget.h"
#include "Blueprint/UserWidget.h"

void AMainHUD::ShowHideScope()
{
	

	if (bScope)
	{
		bScope = false;
		
		if (ScopeWidget != nullptr)
		{
			ScopeWidget->RemoveFromParent();
		}
	}
	else
	{
		bScope = true;
		
		ScopeWidget = Cast<UScopeWidget>(CreateWidget(GetWorld(), ScopeClass));

		if (ScopeWidget != nullptr)
		{
			ScopeWidget->AddToViewport();
		}
		
	}
}
