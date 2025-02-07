// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widget/ISMainMenuWidget.h"
#include "ISMainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISMainMenuController : public APlayerController
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UISMainMenuWidget> MainMenuWidgetRef;
};
