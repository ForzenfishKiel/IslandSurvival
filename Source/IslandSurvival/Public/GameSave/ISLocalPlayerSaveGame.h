// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISPlayerItemDataTable.h"
#include "GameFramework/SaveGame.h"
#include "ISLocalPlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISLocalPlayerSaveGame : public ULocalPlayerSaveGame
{
	GENERATED_BODY()
public:

	//存档名称
	UPROPERTY()
	FString SlotName = FString();

	//存档索引
	int32 SlotIndex = 0;

	//玩家名称
	UPROPERTY()
	FString PlayerName = FString("Default Name");

	/************************** 玩家相关数据 **************************/

	//角色等级
	UPROPERTY()
	int32 PlayerLevel = 1;

	//经验值
	UPROPERTY()
	int32 XP = 0;
	
	//可分配属性点
	UPROPERTY()
	int32 AttributePoints = 0;

	UPROPERTY()
	FTransform PlayerTransform;  //玩家位置

	/************************** 主要属性 **************************/

	UPROPERTY()
	float Health = 0.0f;

	UPROPERTY()
	float Hungry = 0.0f;

	UPROPERTY()
	float Thirsty = 0.0f;

	UPROPERTY()
	float Vigor = 0.0f;

	/************************** 其他数据 **************************/

	UPROPERTY()
	TArray<FItemInformation> Items;  //玩家背包
	UPROPERTY()
	TArray<FItemInformation> HotBarItems; //玩家物品栏
};
