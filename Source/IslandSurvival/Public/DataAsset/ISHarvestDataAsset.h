// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Havesting/ISHarvestingBase.h"
#include "ISHarvestDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISHarvestDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	TSubclassOf<AISHarvestingBase> GetHarvest(const FString& InName) const;
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSubclassOf<AISHarvestingBase>> StaticMeshMap;
};
