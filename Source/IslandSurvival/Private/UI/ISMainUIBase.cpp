// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ISMainUIBase.h"

void UISMainUIBase::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerWasSet();
}
