// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Game/ISGameplayMode.h"
#include "Kismet/GameplayStatics.h"

UISCollectibleDataAsset* UISAbilitysystemLibary::GetCollectibleDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode*SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->CollectibleDataAsset;
}
