// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController/ISGameSaveWidgetController.h"
#include "ISGameSaveSlotWC.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISGameSaveSlotWC : public UISGameSaveWidgetController
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex = -1;  //视图索引对应存档的索引
	
	//当前进入加载存档界面时，此存档应该显示的用户控件界面。
	void SetSlotName(const FString& InSlotName);
	FString GetSlotName() const { return SlotName; };

	void SetPlayerName(const FString& InPlayerName);
	FString GetPlayerName() const { return PlayerName; };

	void SetPlayerLevel(const int32 InPlayerLevel);
	int32 GetPlayerLevel() const { return PlayerLevel; };

	void SetMapName(const FString& InMapName);
	FString GetMapName() const { return MapName; };
private:
	
	//用户控件的名称
	FString SlotName;

	//用户设置的角色名称
	FString PlayerName;

	//角色的等级
	int32 PlayerLevel;

	//当前角色所在的地图名称
	FString MapName;
};
