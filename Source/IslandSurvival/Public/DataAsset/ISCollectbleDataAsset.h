// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Havesting/ISHarvestingBase.h"
#include "ISCollectbleDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISCollectbleDataAsset : public UDataAsset
{
	GENERATED_BODY()

	TSubclassOf<AISHarvestingBase> GetHavestingBaseClass(const FString InName);
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSubclassOf<AISHarvestingBase>> ISCollectbleStaticMeshMap;
};
