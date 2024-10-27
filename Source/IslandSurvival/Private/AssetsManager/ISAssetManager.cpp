// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetsManager/ISAssetManager.h"
#include "Game/ISGameplayTagsManager.h"

UISAssetManager& UISAssetManager::Get()
{
	check(GEngine);
	UISAssetManager*ISAssetManager = Cast<UISAssetManager>(GEngine->AssetManager);
	return *ISAssetManager;
}

void UISAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FGameplayTagsManager::InitializeNativeGameplayTags();
}
