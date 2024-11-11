// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAsset/ISCraftingContainer.h"
#include "ISMenuUIBase.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISMenuUIBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,Category = "UI")
	TObjectPtr<UObject> WidgetController;
	UPROPERTY(BlueprintReadOnly)
	FCraftingInfoContainer CraftingInfo;
	UFUNCTION(Client,Reliable)
	void InitializeCraftingData(UObject* Target);
	UFUNCTION(Client,Reliable)
	void SafeToClearData();
	void SetWidgetController(UObject*TargetWidgetController);
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerWasSet();
};
