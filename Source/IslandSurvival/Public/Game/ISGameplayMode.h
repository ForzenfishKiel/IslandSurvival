// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISCollectibleDataAsset.h"
#include "DataAsset/ISEquipableDataAsset.h"
#include "DataAsset/ISStorageDataConfig.h"
#include "GameFramework/GameModeBase.h"
#include "ISGameplayMode.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISGameplayMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UISCollectibleDataAsset> CollectibleDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UISEquipableDataAsset> EquipableDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UISStorageDataConfig>StorageDataConfig;
};
