// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interface/ISContainerInterface.h"
#include "ISStorageDataConfig.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName ContainerName;  //背包名字
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 InventorySpace = 0;  //背包空间
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EContainerType ContainerType = EContainerType::None;  //库存类型
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bReplicates = false;  //库存是否同步？
};
USTRUCT(BlueprintType)
struct FStorageDataConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FInventoryConfig> Inventory;
};
UCLASS()
class ISLANDSURVIVAL_API UISStorageDataConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TMap<EContainerType,FStorageDataConfig> InventoryConfigMap;
	FStorageDataConfig* GetInventoryConfig(EContainerType ContainerType);
};
