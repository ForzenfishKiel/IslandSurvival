// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/ISGameInstance.h"
#include "ISGameSaveWidgetController.generated.h"

/**
 * 
 */
//保存场景中的Actor结构体
class UISGameSaveSlotWC;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotWasLoaded,FISSaveGames, Slots);

UCLASS(BlueprintType, Blueprintable)
class ISLANDSURVIVAL_API UISGameSaveWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void LoadGameSaveSlot() const;
	UPROPERTY(BlueprintAssignable)
	FOnSlotWasLoaded OnSlotWasLoaded;
	UFUNCTION(BlueprintCallable)
	void WhenGameStartButtonWasPressed();  //当进入游戏的按钮被点击时
	UFUNCTION(BlueprintCallable)
	void LoadGameButtonWasPressed(const int32 InIndex,const FString InSlotName);
	UFUNCTION(BlueprintCallable)
	void WhenLoadGameSlotDeleteButtonWasPressed(const int32 InIndex); //当删除存档的按钮被点击时
	UFUNCTION(BlueprintCallable)
	void OnPlayerNameWasInput(const FString InPlayerName);
		
	UPROPERTY()
	TMap<int32,UISGameSaveSlotWC*> LoadSlots;  //保存着存档的查找数据
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UISGameSaveSlotWC> GameSaveSlotClass;
	UPROPERTY()
	FString PlayerNameSave;
};
