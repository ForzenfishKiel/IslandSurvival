// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/ISMainMenuController.h"

#include "MainMenu/ISMainMenuHUD.h"

void AISMainMenuController::BeginPlay()
{
	Super::BeginPlay();
	MainMenuWidgetRef = CreateWidget<UISMainMenuWidget>(GetWorld(), MainMenuWidgetClass);

	if(!HasAuthority())
	{
		MainMenuWidgetRef->AddToViewport();  //添加到玩家视口
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());  //设置游戏模式为仅UI
	}
}
