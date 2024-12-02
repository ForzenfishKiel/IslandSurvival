// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ISBuildingDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBuildingInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 BuildHP = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Building")
	int32 BuildMaxHP = 0;
};
UCLASS()
class ISLANDSURVIVAL_API UISBuildingDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
};
