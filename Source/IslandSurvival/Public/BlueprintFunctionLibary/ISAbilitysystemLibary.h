// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISDropCollectibleDataAsset.h"
#include "DataAsset/ISCharacterClassInfo.h"
#include "DataAsset/ISCraftingContainer.h"
#include "DataAsset/ISEquipableDataAsset.h"
#include "DataAsset/ISHarvestDataAsset.h"
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
	static UISDropCollectibleDataAsset* GetDropCollectibleDataAsset(const UObject* WorldContextObject);
	static UISEquipableDataAsset* GetEquipableDataAsset(const UObject* WorldContextObject);
	static UISWeaponDataAsset* GetSweaponDataAsset(const UObject* WorldContextObject);
	static UISHarvestDataAsset* GetHarvestDataAsset(const UObject* WorldContextObject);
	static UISCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	static void InitializeCharacterAttributes(const UObject* WorldContextObject,FName InName,float Level,UAbilitySystemComponent* ASC);
	static void GiveStartupAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC,FName InName);
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject,FName EnemyName,int32 CharacterLevel);
};
