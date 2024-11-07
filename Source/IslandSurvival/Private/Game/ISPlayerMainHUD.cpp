// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISPlayerMainHUD.h"



UISMainUIWidgetController* AISPlayerMainHUD::GetMainUIWidgetController(const FCharacterParams& CharacterParams)
{
	if(IsMainUIWidgetController==nullptr)
	{
		IsMainUIWidgetController = NewObject<UISMainUIWidgetController>(GetWorld(),ISMainUIWidgetControllerClass);
		IsMainUIWidgetController->SetWidgetControllerParams(CharacterParams);
		IsMainUIWidgetController->BindCallBackDependencies();
	}
	return IsMainUIWidgetController;
}
void AISPlayerMainHUD::InitUserWidget(const FCharacterParams& CharacterParams)
{
	UISMainUIWidgetController* MainUIWidgetController = GetMainUIWidgetController(CharacterParams);
	IsMainUI = CreateWidget<UISMainUIBase>(GetWorld(),ISMainUIClass);
	ISMenuUI = CreateWidget<UISMenuUIBase>(GetWorld(),ISMenuClass); //创建人物的背包界面UI
	IsMainUI->SetWidgetController(MainUIWidgetController);
	IsMainUI->AddToViewport();
}

