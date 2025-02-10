// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/Widget/ISMainMenuWidget.h"

void UISMainMenuWidget::WidgetControllerSet(UISGameSaveWidgetController* InWidgetController)
{
	GameSaveWidgetController = InWidgetController;
	OnWidgetControllerSet();
}
