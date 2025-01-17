// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingSystem/ISBuildingSystemBase.h"
#include "WidgetController/ISWidgetController.h"
#include "ISMainUIBase.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISMainUIBase : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetWidgetController(UObject*InWidgetController);
	UPROPERTY(BlueprintReadOnly, Category = "UIModel")
	TObjectPtr<UObject> WidgetController;
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerWasSet();
	UFUNCTION(BlueprintImplementableEvent)
	void SendObjectInfo(UObject* TargetBuilding);
	UFUNCTION(BlueprintImplementableEvent)
	void SendTickTraceWasEnd();
};
