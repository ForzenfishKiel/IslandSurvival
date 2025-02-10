// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/ISMainMenuHUD.h"

#include "Blueprint/UserWidget.h"

void AISMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	GameSaveWidgetControllerRef = NewObject<UISGameSaveWidgetController>(this,GameSaveWidgetControllerClass);
	
	MainMenuWidgetRef = CreateWidget<UISMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidgetRef->WidgetControllerSet(GameSaveWidgetControllerRef);  //加载WidgetController配置
	MainMenuWidgetRef->AddToViewport();

	APlayerController* PC = GetOwningPlayerController();
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidgetRef->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(true);

	GameSaveWidgetControllerRef->LoadGameSaveSlot();  //加载游戏保存插槽
}
