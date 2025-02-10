// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetController/ISGameSaveWidgetController.h"
#include "ISMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UISGameSaveWidgetController> GameSaveWidgetController;
	void WidgetControllerSet(UISGameSaveWidgetController* InWidgetController);
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
};
