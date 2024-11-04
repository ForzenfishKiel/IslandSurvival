// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISCraftItemInformation.h"
#include "Engine/DataAsset.h"
#include "Interface/ISContainerInterface.h"
#include "ISCraftingContainer.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCraftingInfoContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName TargetName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UDataTable> ItemCraftingInfoDataTable;
};
USTRUCT(BlueprintType)
struct FCraftingArray
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FCraftingInfoContainer> CraftingInfos;
};
UCLASS()
class ISLANDSURVIVAL_API UISCraftingContainer : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EContainerType,FCraftingArray>CraftingInfoMap;
	FCraftingArray GetCraftingInfoMap(EContainerType WorkSpaceType);
};
