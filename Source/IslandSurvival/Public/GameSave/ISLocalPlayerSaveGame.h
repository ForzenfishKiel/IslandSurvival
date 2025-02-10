// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
};
