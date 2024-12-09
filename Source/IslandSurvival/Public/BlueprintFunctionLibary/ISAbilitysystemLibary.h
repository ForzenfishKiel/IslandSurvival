// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISCollectibleDataAsset.h"
#include "DataAsset/ISCraftingContainer.h"
#include "DataAsset/ISEquipableDataAsset.h"
#include "DataAsset/ISStorageDataConfig.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ISAbilitysystemLibary.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISAbilitysystemLibary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static UISCollectibleDataAsset* GetCollectibleDataAsset(const UObject* WorldContextObject);
	static UISEquipableDataAsset*GetEquipableDataAsset(const UObject* WorldContextObject);
	static UISStorageDataConfig* GetStorageDataConfig(const UObject* WorldContextObject);
	static FItemInformation* FindTargetItemData(const UObject* WorldContextObject,const FName TargetRowName);
};
