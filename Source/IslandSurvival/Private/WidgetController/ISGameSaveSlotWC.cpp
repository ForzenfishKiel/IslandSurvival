// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/ISGameSaveSlotWC.h"

void UISGameSaveSlotWC::SetSlotName(const FString& InSlotName)
{
	SlotName = InSlotName;
}

void UISGameSaveSlotWC::SetPlayerName(const FString& InPlayerName)
{
	PlayerName = InPlayerName;
}

void UISGameSaveSlotWC::SetPlayerLevel(const int32 InPlayerLevel)
{
	PlayerLevel = InPlayerLevel;
}

void UISGameSaveSlotWC::SetMapName(const FString& InMapName)
{
	MapName = InMapName;
}
