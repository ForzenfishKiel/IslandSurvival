// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISDropCollectibleDataAsset.h"
#include "DataAsset/ISCharacterClassInfo.h"
#include "DataAsset/ISEquipableDataAsset.h"
#include "DataAsset/ISHarvestDataAsset.h"
#include "DataAsset/ISTraderSpecialData.h"
#include "DataAsset/ISWeaponDataAsset.h"
#include "GameFramework/GameModeBase.h"
#include "GameSave/ISLocalPlayerSaveGame.h"
#include "WidgetController/ISGameSaveSlotWC.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UISHarvestDataAsset> HarvestDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UISCharacterClassInfo> ISCharacterClassInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UISTraderSpecialData> ISTraderSpecialData;

	UPROPERTY()
	TSet<TWeakObjectPtr<APlayerController>> LoginPlayerList;

	APlayerController* FindPlayerControllerFromLocalPlayer(ULocalPlayer* InLocalPlayer) const;

	void SaveSlotData(const UISGameSaveSlotWC* LoadSlot,int32 SlotIndex) const;

	UISLocalPlayerSaveGame* GetSaveSlotData(const FString& InSlotName , int32 SlotIndex) const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;
};
