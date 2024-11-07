// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ISCharacterBase.h"
#include "GameFramework/HUD.h"
#include "UI/ISMainUIBase.h"
#include "UI/ISMenuUIBase.h"
#include "WidgetController/ISMainUIWidgetController.h"
#include "ISPlayerMainHUD.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISPlayerMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	void InitUserWidget(const FCharacterParams&CharacterParams);
	UISMainUIWidgetController* GetMainUIWidgetController(const FCharacterParams&CharacterParams);
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ISMenuClass;
	UPROPERTY()
	TObjectPtr<UISMenuUIBase> ISMenuUI;  //创建角色的背包UI对象
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ISMainUIClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UISMainUIBase> IsMainUI;
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UISMainUIWidgetController>ISMainUIWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UISMainUIWidgetController> IsMainUIWidgetController;
};
