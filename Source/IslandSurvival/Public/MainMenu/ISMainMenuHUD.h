// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widget/ISMainMenuWidget.h"
#include "WidgetController/ISGameSaveWidgetController.h"
#include "ISMainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISMainMenuHUD : public AHUD
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UISMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UISMainMenuWidget> MainMenuWidgetRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UISGameSaveWidgetController> GameSaveWidgetControllerClass; //加载主界面视图模型

	UPROPERTY()
	TObjectPtr<UISGameSaveWidgetController> GameSaveWidgetControllerRef;
};
