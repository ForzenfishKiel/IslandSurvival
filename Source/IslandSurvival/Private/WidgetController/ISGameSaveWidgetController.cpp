// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/ISGameSaveWidgetController.h"
#include "Game/ISGameInstance.h"
#include "WidgetController/ISGameSaveSlotWC.h"
#include "Game/ISGameplayMode.h"
#include "Kismet/GameplayStatics.h"

//加载存档配置
void UISGameSaveWidgetController::LoadGameSaveSlot() const
{
	AISGameplayMode* GameMode = Cast<AISGameplayMode>( UGameplayStatics::GetGameMode(this));
	UISGameInstance* ISGameInstance = Cast<UISGameInstance>( UGameplayStatics::GetGameInstance(this));

	for(auto Iter : ISGameInstance->SaveGames)
	{
		const UISLocalPlayerSaveGame* SaveGame = GameMode->GetSaveSlotData(Iter.SlotName,Iter.SlotIndex);
		if(SaveGame)
		{
			OnSlotWasLoaded.Broadcast(Iter); //将值传递出去
		}
	}
}

//当玩家开始进入游戏
void UISGameSaveWidgetController::WhenGameStartButtonWasPressed()
{
	AISGameplayMode* GameplayMode = Cast<AISGameplayMode>( UGameplayStatics::GetGameMode(this));
	
	UISGameInstance* ISGameInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(this));
	
	UISGameSaveSlotWC* GameSaveSlot = NewObject<UISGameSaveSlotWC>(this,GameSaveSlotClass);
	GameSaveSlot->SetPlayerName(PlayerNameSave);
	GameSaveSlot->SetPlayerLevel(1);  //设定玩家的等级
	GameSaveSlot->SetMapName(GameplayMode->DefaultMapName); //设置地图名称
	GameSaveSlot->SetSlotName(GameplayMode->DefaultMapName);
	GameSaveSlot->SlotIndex = LoadSlots.Num();  //索引等于存储长度，也就是未开辟的一位
	GameplayMode->SaveSlotData(GameSaveSlot,GameSaveSlot->SlotIndex);  //保存新的存档

	ISGameInstance->SlotIndex = GameSaveSlot->SlotIndex;
	ISGameInstance->LoadSlotName = GameSaveSlot->GetSlotName();  //全局游戏保存当前游玩的存档
	
	FISSaveGames SaveGame;
	SaveGame.SlotIndex = GameSaveSlot->SlotIndex;
	SaveGame.SlotName = GameSaveSlot->GetSlotName();
	SaveGame.PlayerName = PlayerNameSave;
	ISGameInstance->SaveGames.Emplace(SaveGame);
	
	ISGameInstance->bFirstTimeStartGame = true;  //确认为第一次进入游戏
	
	GameplayMode->TravelToMap(GameplayMode->DefaultMapName);
}

//加载存档的按钮被点击的时候
void UISGameSaveWidgetController::LoadGameButtonWasPressed(const int32 InIndex , const FString InSlotName)
{
	AISGameplayMode* GameplayMode = Cast<AISGameplayMode>( UGameplayStatics::GetGameMode(this));
	UISGameInstance* ISGameInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(this));

	
	FISSaveGames SlotSaveGame;
	SlotSaveGame.SlotIndex = InIndex;
	SlotSaveGame.SlotName = InSlotName;

	if(ISGameInstance->SaveGames.Contains(SlotSaveGame) && GameplayMode->GetSaveSlotData(InSlotName,InIndex))
	{
		ISGameInstance->LoadSlotName = InSlotName;
		ISGameInstance->SlotIndex = InIndex;
		ISGameInstance->bFirstTimeStartGame = false;  //不是第一次进入游戏
		
		GameplayMode->TravelToMap(InSlotName);  //打开地图
	}
}

//当角色输入名称时
void UISGameSaveWidgetController::OnPlayerNameWasInput(const FString InPlayerName)
{
	PlayerNameSave = InPlayerName;
}

//删除存档插槽的按钮被点击的时候
void UISGameSaveWidgetController::WhenLoadGameSlotDeleteButtonWasPressed(const int32 InIndex)
{
	AISGameplayMode* GameplayMode = Cast<AISGameplayMode>( UGameplayStatics::GetGameMode(this));// 获取游戏模式
	UISGameInstance* ISGameInstance = Cast<UISGameInstance>( UGameplayStatics::GetGameInstance(this));

	

	for(auto Iter : ISGameInstance->SaveGames)
	{
		if(Iter.SlotIndex == InIndex)
		{
			GameplayMode->DeleteSlotData(Iter.SlotName,Iter.SlotIndex);
			ISGameInstance->SaveGames.Remove(Iter);
		}
	}
}
