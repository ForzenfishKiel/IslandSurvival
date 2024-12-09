// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"

#include "Game/ISGameInstance.h"
#include "Game/ISGameplayMode.h"
#include "Kismet/GameplayStatics.h"

UISCollectibleDataAsset* UISAbilitysystemLibary::GetCollectibleDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode* SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->CollectibleDataAsset;
}

UISEquipableDataAsset* UISAbilitysystemLibary::GetEquipableDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode* SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->EquipableDataAsset;
}

UISStorageDataConfig* UISAbilitysystemLibary::GetStorageDataConfig(const UObject* WorldContextObject)
{
	AISGameplayMode* SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->StorageDataConfig;
}

FItemInformation* UISAbilitysystemLibary::FindTargetItemData(const UObject* WorldContextObject,const FName TargetRowName)
{
	UISGameInstance* SourceGameInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if(!SourceGameInstance) return nullptr;
	FItemInformation* TargetData = SourceGameInstance->ItemDataTable->FindRow<FItemInformation>(TargetRowName,TEXT("name"));
	if(!TargetData) return nullptr;
	return TargetData;
}
