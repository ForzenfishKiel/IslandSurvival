// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/ISGameSaveWidgetController.h"
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
	UISGameSaveSlotWC* GameSaveSlot = NewObject<UISGameSaveSlotWC>(this,GameSaveSlotClass);
	GameSaveSlot->SetPlayerName(StartingConfig.PlayerName);
	GameSaveSlot->SetPlayerLevel(1);  //设定玩家的等级
	GameSaveSlot->SetMapName(FString("IsLand")); //设置地图名称
	GameSaveSlot->SetSlotName(FString("SaveData"));
	GameSaveSlot->SlotIndex = LoadSlots.Num();  //索引等于存储长度，也就是未开辟的一位
	GameplayMode->SaveSlotData(GameSaveSlot,GameSaveSlot->SlotIndex);  //保存新的存档
	LoadSlots.Add(LoadSlots.Num(),GameSaveSlot);
}

//当角色输入名称时
void UISGameSaveWidgetController::OnPlayerNameWasInput(const FString InPlayerName)
{
	StartingConfig.PlayerName = InPlayerName;
}
