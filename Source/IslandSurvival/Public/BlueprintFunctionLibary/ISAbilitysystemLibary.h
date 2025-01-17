// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISDropCollectibleDataAsset.h"
#include "DataAsset/ISCollectbleDataAsset.h"
#include "DataAsset/ISCraftingContainer.h"
#include "DataAsset/ISEquipableDataAsset.h"
#include "DataAsset/ISWeaponDataAsset.h"
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
	static UISDropCollectibleDataAsset* GetCollectibleDataAsset(const UObject* WorldContextObject);
	static UISEquipableDataAsset* GetEquipableDataAsset(const UObject* WorldContextObject);
	static UISWeaponDataAsset* GetSweaponDataAsset(const UObject* WorldContextObject);
	static UISCollectbleDataAsset* GetISCollectibleDataAsset(const UObject* WorldContextObject);
};
