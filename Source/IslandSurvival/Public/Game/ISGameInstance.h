// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/ISCraftingContainer.h"
#include "Engine/GameInstance.h"
#include "ISGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FISSaveGames
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	FString SlotName;
	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;
	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;  //加载玩家数据
	UPROPERTY(BlueprintReadOnly)
	int32 PlayerLevel = -1;

	bool operator==(const FISSaveGames& Other) const
	{
		// 根据实际需要比较关键字段
		return SlotName == Other.SlotName && SlotIndex == Other.SlotIndex;
	}
};
UCLASS()
class ISLANDSURVIVAL_API UISGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataTable")
	TObjectPtr<UDataTable>ItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataTable")
	TObjectPtr<UISCraftingContainer>ItemContainer;

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();  //保存存档使用的名称

	UPROPERTY()
	FString LoadMapName = FString();

	UPROPERTY()
	int32 SlotIndex = -1;  //保存存档使用的索引

	//是否为第一次进入游戏
	bool bFirstTimeStartGame = false;

	//是否为多人模式启动
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsMultPlayerMode = false;

	UPROPERTY()
	TArray<FISSaveGames> SaveGames;
};
