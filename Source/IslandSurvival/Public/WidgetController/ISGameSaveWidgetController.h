// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISGameSaveWidgetController.generated.h"

/**
 * 
 */
//创建玩家在主界面的配置
USTRUCT(BlueprintType)
struct FStartingConfig
{
	GENERATED_BODY()
	UPROPERTY()
	FString PlayerName;  //加载玩家数据
	
};
class UISGameSaveSlotWC;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotWasLoaded,UISGameSaveSlotWC*, Slot);

UCLASS(BlueprintType, Blueprintable)
class ISLANDSURVIVAL_API UISGameSaveWidgetController : public UObject
{
	GENERATED_BODY()
public:
	void LoadGameSaveSlot() const;
	UPROPERTY(BlueprintAssignable)
	FOnSlotWasLoaded OnSlotWasLoaded;
	UPROPERTY(BlueprintReadOnly)
	FStartingConfig StartingConfig;
	UFUNCTION(BlueprintCallable)
	void WhenGameStartButtonWasPressed();  //当进入游戏的按钮被点击时
	UFUNCTION(BlueprintCallable)
	void OnPlayerNameWasInput(const FString InPlayerName);
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UISGameSaveSlotWC> GameSaveSlotClass;
	UPROPERTY()
	TMap<int32,UISGameSaveSlotWC*> LoadSlots;  //保存着存档的查找数据
};
