// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/ISMainMenuHUD.h"

#include "Blueprint/UserWidget.h"

void AISMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if(! HasAuthority())
	{
		check(MainMenuWidgetClass);
		MainMenuWidgetRef = CreateWidget<UISMainMenuWidget>(GetOwningPlayerController(), MainMenuWidgetClass);

		MainMenuWidgetRef->AddToViewport();  //添加到玩家视口
		GetOwningPlayerController()->SetShowMouseCursor(true);
		GetOwningPlayerController()->SetInputMode(FInputModeUIOnly());  //设置游戏模式为仅UI
	}
}
