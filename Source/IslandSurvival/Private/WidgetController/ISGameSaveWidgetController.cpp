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

	for(auto Iter = LoadSlots.CreateConstIterator();Iter; ++Iter)
	{
		const UISLocalPlayerSaveGame* SaveGame = GameMode->GetSaveSlotData(Iter.Value()->GetSlotName(),Iter.Key());
		if(SaveGame)
		{
			OnSlotWasLoaded.Broadcast(Iter.Value()); //将值传递出去
		}
	}
	
}

//当玩家开始进入游戏
void UISGameSaveWidgetController::WhenGameStartButtonWasPressed()
{
	AISGameplayMode* GameplayMode = Cast<AISGameplayMode>( UGameplayStatics::GetGameMode(this));
	
	UISGameInstance* ISGameInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(this));
	
	UISGameSaveSlotWC* GameSaveSlot = NewObject<UISGameSaveSlotWC>(this,GameSaveSlotClass);
	GameSaveSlot->SetPlayerName(StartingConfig.PlayerName);
	GameSaveSlot->SetPlayerLevel(1);  //设定玩家的等级
	GameSaveSlot->SetMapName(GameplayMode->DefaultMapName); //设置地图名称
	GameSaveSlot->SetSlotName(StartingConfig.PlayerName);
	GameSaveSlot->SlotIndex = LoadSlots.Num();  //索引等于存储长度，也就是未开辟的一位
	GameplayMode->SaveSlotData(GameSaveSlot,GameSaveSlot->SlotIndex);  //保存新的存档

	ISGameInstance->SlotIndex = GameSaveSlot->SlotIndex;
	ISGameInstance->LoadSlotName = GameSaveSlot->GetSlotName();  //全局游戏保存当前游玩的存档
	ISGameInstance->bFirstTimeStartGame = true;  //确认为第一次进入游戏
	
	LoadSlots.Emplace(LoadSlots.Num(),GameSaveSlot);  //存档插槽中添加存档
	LoadGameSaveSlot();  //存档插槽加载存档


	GameplayMode->TravelToMap(GameSaveSlot);
}

//加载存档的按钮被点击的时候
void UISGameSaveWidgetController::LoadGameButtonWasPressed(const int32 InIndex)
{
	if(LoadSlots.Contains(InIndex))
	{
		AISGameplayMode* GameplayMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(this));

		GameplayMode->TravelToMap(LoadSlots[InIndex]);
	}
}

//当角色输入名称时
void UISGameSaveWidgetController::OnPlayerNameWasInput(const FString InPlayerName)
{
	StartingConfig.PlayerName = InPlayerName;
}

//删除存档插槽的按钮被点击的时候
void UISGameSaveWidgetController::WhenLoadGameSlotDeleteButtonWasPressed(const int32 InIndex)
{
	AISGameplayMode* GameplayMode = Cast<AISGameplayMode>( UGameplayStatics::GetGameMode(this));// 获取游戏模式

	GameplayMode->DeleteSlotData(LoadSlots[InIndex]->GetSlotName(),InIndex);

	LoadSlots.Remove(InIndex);  //删除对应索引的存档插槽
}
