// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/ISCraftingContainer.h"
#include "Engine/GameInstance.h"
#include "ISGameInstance.generated.h"

/**
 * 
 */
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
	int32 SlotIndex = -1;  //保存存档使用的索引

	//是否为第一次进入游戏
	bool bFirstTimeStartGame = false;
};
