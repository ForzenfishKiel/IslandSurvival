// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISDropCollectibleDataAsset.h"
#include "DataAsset/ISEquipableDataAsset.h"
#include "DataAsset/ISWeaponDataAsset.h"
#include "GameFramework/GameModeBase.h"
#include "ISGameplayMode.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISGameplayMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	virtual void PostLogin(APlayerController* NewPlayer) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UISDropCollectibleDataAsset> CollectibleDropDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UISEquipableDataAsset> EquipableDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UISWeaponDataAsset> WeaponDataAsset;

	UPROPERTY()
	TSet<TWeakObjectPtr<APlayerController>> LoginPlayerList;

	APlayerController* FindPlayerControllerFromLocalPlayer(ULocalPlayer* InLocalPlayer);
};
