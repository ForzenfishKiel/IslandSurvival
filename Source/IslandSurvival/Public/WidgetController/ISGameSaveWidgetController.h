// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/ISGameInstance.h"
#include "ISGameSaveWidgetController.generated.h"

/**
 * 
 */
//保存场景中的Actor结构体
USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform Transform = FTransform();

	//Actor身上序列号的数据，必须通过UPROPERTY定义过，只在保存存档时使用。
	UPROPERTY()
	TArray<uint8> Bytes;
};

//自定义运算符==，如果结构体内的ActorName相同，这代表这两个结构体为相同结构体
inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

//地图相关数据保存
USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapAssetName = FString();

	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};
//创建玩家在主界面的配置
USTRUCT(BlueprintType)
struct FStartingConfig
{
	GENERATED_BODY()
	UPROPERTY()
	FString PlayerName;  //加载玩家数据
	
};
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
	UPROPERTY(BlueprintReadOnly)
	FStartingConfig StartingConfig;
	UFUNCTION(BlueprintCallable)
	void WhenGameStartButtonWasPressed();  //当进入游戏的按钮被点击时
	UFUNCTION(BlueprintCallable)
	void LoadGameButtonWasPressed(const int32 InIndex);
	UFUNCTION(BlueprintCallable)
	void WhenLoadGameSlotDeleteButtonWasPressed(const int32 InIndex); //当删除存档的按钮被点击时
	UFUNCTION(BlueprintCallable)
	void OnPlayerNameWasInput(const FString InPlayerName);
		
	UPROPERTY()
	TMap<int32,UISGameSaveSlotWC*> LoadSlots;  //保存着存档的查找数据
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UISGameSaveSlotWC> GameSaveSlotClass;
};
