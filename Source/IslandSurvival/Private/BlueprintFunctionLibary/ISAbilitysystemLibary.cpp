// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Game/ISGameplayMode.h"
#include "Kismet/GameplayStatics.h"

UISDropCollectibleDataAsset* UISAbilitysystemLibary::GetDropCollectibleDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode*SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->CollectibleDropDataAsset;
}

UISEquipableDataAsset* UISAbilitysystemLibary::GetEquipableDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode*SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->EquipableDataAsset;
}

UISWeaponDataAsset* UISAbilitysystemLibary::GetSweaponDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode*SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->WeaponDataAsset;
}

UISHarvestDataAsset* UISAbilitysystemLibary::GetHarvestDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode*SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->HarvestDataAsset;
}
