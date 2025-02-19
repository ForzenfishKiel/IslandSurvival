// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
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
	TObjectPtr<UObject> WidgetController = nullptr;
	
	UPROPERTY(BlueprintReadOnly,Category = "UI")
	TObjectPtr<UObject> WidgetOwner = nullptr;
	
	UPROPERTY(BlueprintReadOnly,Category = "UI")
	TObjectPtr<UObject> WidgetControllerOwner = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	FCraftingInfoContainer CraftingInfo;
	
	UFUNCTION(Client,Reliable)
	void InitializeCraftingData(UObject* Target);
	
	UFUNCTION(Client,Reliable)
	void SafeToClearData();
	
	void SetWidgetController(UObject*TargetWidgetController);
	
	void SetWidgetOwner(UObject*TargetWidgetOwner);
	
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerWasSet();
	
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetOwnerWasSet();
	
	void ImportGameItemTableData(); //导入游戏的物品表
	UFUNCTION(BlueprintImplementableEvent)
	void ItemTableDataWasSet();
};
