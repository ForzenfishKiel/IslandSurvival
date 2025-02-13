// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISGameplayMode.h"

#include "CookOnTheSide/CookOnTheFlyServer.h"
#include "Game/ISGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AISGameplayMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	LoginPlayerList.Add(NewPlayer);  //当客户端登录服务器时，向数组导入
}

APlayerController* AISGameplayMode::FindPlayerControllerFromLocalPlayer(ULocalPlayer* InLocalPlayer) const
{
	if(LoginPlayerList.Contains(InLocalPlayer->GetPlayerController(GetWorld())))
	{
		return InLocalPlayer->GetPlayerController(GetWorld());
	}
	return nullptr;
}

void AISGameplayMode::SaveSlotData(const UISGameSaveSlotWC* LoadSlot, int32 SlotIndex) const
{
	//根据内部存档的名称和索引查找是否存在该存档
	if(UGameplayStatics::DoesSaveGameExist(LoadSlot->GetSlotName(),SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetSlotName(),SlotIndex);  //删除该存档
	}

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);  //创建一个新的存档对象
	UISLocalPlayerSaveGame* LocalPlayerSaveGame = Cast<UISLocalPlayerSaveGame>(SaveGameObject);

	LocalPlayerSaveGame->PlayerName = LoadSlot->GetSlotName();
	LocalPlayerSaveGame->SlotName = LoadSlot->GetSlotName();
	LocalPlayerSaveGame->SlotIndex = SlotIndex;

	UGameplayStatics::SaveGameToSlot(LocalPlayerSaveGame,LoadSlot->GetSlotName(),SlotIndex);  //保存一个新的存档
}



void AISGameplayMode::TravelToMap(UISGameSaveSlotWC* LoadSlot)
{
	const FString SlotName = LoadSlot->GetSlotName();
	const int32 SlotIndex = LoadSlot->SlotIndex;
	
	//打开地图
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot, LoadingMap.FindChecked(LoadSlot->GetMapName()));
	
}

void AISGameplayMode::ReturnToMainMenu() const
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainUIMap);
}

void AISGameplayMode::DeleteSlotData(const FString& InSlotName, int32 InSlotIndex)
{
	//检查是否有对应名称的存档
	if(UGameplayStatics::DoesSaveGameExist(InSlotName, InSlotIndex))
	{
		//删除已保存的存档
		UGameplayStatics::DeleteGameInSlot(InSlotName, InSlotIndex);
	}
}

UISLocalPlayerSaveGame* AISGameplayMode::GetSaveSlotData(const FString& InSlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject;
	if(UGameplayStatics::DoesSaveGameExist(InSlotName,SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(InSlotName,SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);  //创建一个新的存档
	}
	UISLocalPlayerSaveGame* LocalPlayerSaveGame = Cast<UISLocalPlayerSaveGame>(SaveGameObject);
	
	return LocalPlayerSaveGame;
}

void AISGameplayMode::BeginPlay()
{
	Super::BeginPlay();

	LoadingMap.Emplace(DefaultMapName,DefaultMap);
}

//保存当前游戏实例的游戏进度
void AISGameplayMode::SaveInGameProgressData(UISLocalPlayerSaveGame* SaveObject) const
{
	UISGameInstance* ISGameInstance = Cast<UISGameInstance>(GetGameInstance());

	//从游戏实例获取到存档名称和索引
	const FString InGameLoadSlotName = ISGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = ISGameInstance->SlotIndex;

	//保存存档
	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}
//获取当前使用的存档
UISLocalPlayerSaveGame* AISGameplayMode::RetrieveInGameSaveData() const
{
	const UISGameInstance* ISGameInstance = Cast<UISGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = ISGameInstance->LoadSlotName;
	const int32 InGameSlotIndex = ISGameInstance->SlotIndex;

	return GetSaveSlotData(InGameLoadSlotName,InGameSlotIndex);
}