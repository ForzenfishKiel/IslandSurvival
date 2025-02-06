// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widget/ISMainMenuWidget.h"
#include "ISMainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISMainMenuHUD : public AHUD
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UISMainMenuWidget> MainMenuWidgetRef;
	virtual void BeginPlay() override;
};
