// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISPlayerMainHUD.h"

void AISPlayerMainHUD::InitUserWidget(const FCharacterParams& CharacterParams)
{
	IsMainUI = CreateWidget<UISMainUIBase>(GetWorld(),ISMainUIClass);
	ISMenuUI = CreateWidget<UISMenuUIBase>(GetWorld(),ISMenuClass); //创建人物的背包界面UI
	IsMainUI->AddToViewport();
}
