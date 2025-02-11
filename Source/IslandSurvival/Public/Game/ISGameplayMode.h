// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISDropCollectibleDataAsset.h"
#include "DataAsset/ISCharacterClassInfo.h"
#include "DataAsset/ISEquipableDataAsset.h"
#include "DataAsset/ISHarvestDataAsset.h"
#include "DataAsset/ISTraderSpecialData.h"
#include "DataAsset/ISWeaponDataAsset.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GameSave/ISLocalPlayerSaveGame.h"
#include "WidgetController/ISGameSaveSlotWC.h"
#include "ISGameplayMode.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISGameplayMode : public AGameMode
{
	GENERATED_BODY()
private:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
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
	void TravelToMap(UISGameSaveSlotWC* LoadSlot);  //将玩家传送到对应的地图
	static void DeleteSlotData(const FString& InSlotName , int32 InSlotIndex);

	UISLocalPlayerSaveGame* GetSaveSlotData(const FString& InSlotName , int32 SlotIndex) const;

	UPROPERTY(EditDefaultsOnly , Category = "LoadingGameConfig")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly, Category = "LoadingGameConfig")
	FString DefaultMapName; //默认地图名称

	UPROPERTY(EditDefaultsOnly, Category = "LoadingGameConfig")
	TSoftObjectPtr<UWorld> DefaultMap; //默认地图资源

	UPROPERTY(EditDefaultsOnly, Category = "LoadingGameConfig")
	FName DefaultPlayerStartTag; //玩家默认出生Tag

	UPROPERTY(EditDefaultsOnly, Category = "LoadingGameConfig")
	TMap<FString,TSoftObjectPtr<UWorld>> LoadingMap; //加载地图名称和对应的映射
}; 
