// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Items/ISItemBase.h"
#include "ISEquipableDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEquipRarityInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	EItemRarity ItemRarity;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FScalableFloat BootMultiplier;  //加成倍率
};
USTRUCT(BlueprintType)
struct FEquipItemRarityContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<FEquipRarityInfo> RarityContainer;
};

UCLASS()
class ISLANDSURVIVAL_API UISEquipableDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category="Defaults")
	TMap<FName,FEquipItemRarityContainer> EquipRarityMap;
	FEquipItemRarityContainer GetTargetItemRarityContainer(FName TargetID);
};
