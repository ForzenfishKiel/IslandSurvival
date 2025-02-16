// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISGameplayMode.h"
#include "EngineUtils.h"

#include "Game/ISGameInstance.h"
#include "Interface/ISSaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

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

	LocalPlayerSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LocalPlayerSaveGame->SlotName = LoadSlot->GetSlotName();
	LocalPlayerSaveGame->SlotIndex = SlotIndex;

	UGameplayStatics::SaveGameToSlot(LocalPlayerSaveGame,LoadSlot->GetPlayerName(),SlotIndex);  //保存一个新的存档
}



void AISGameplayMode::TravelToMap(const FString LoadSlotName)
{
	
	//打开地图
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, LoadingMap.FindChecked(LoadSlotName));
	
}

//返回到主界面
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

//获取存档数据
UISLocalPlayerSaveGame* AISGameplayMode::GetSaveSlotData(const FString& InSlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject;
	if(UGameplayStatics::DoesSaveGameExist(InSlotName,SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(InSlotName,SlotIndex);
		UISLocalPlayerSaveGame* LocalPlayerSaveGame = Cast<UISLocalPlayerSaveGame>(SaveGameObject);
	
		return LocalPlayerSaveGame;
	}
	return nullptr;
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
//保存地图状态
void AISGameplayMode::SaveWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();  //获取地图名称
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UISGameInstance* ISGameInstance = Cast<UISGameInstance>(GetGameInstance());
	check(ISGameInstance);  //断言游戏实例，也就是禁止玩家关闭游戏

	//从当前游戏实例获取当前游戏存档
	if(UISLocalPlayerSaveGame* SaveGame = GetSaveSlotData(ISGameInstance->LoadSlotName,ISGameInstance->SlotIndex))
	{
		//如果保存的游戏对象里没有储存着对应的地图名称
		if(!SaveGame->HasMap(WorldName))
		{
			//如果存档不包含对应关卡内容，将创建一个对应的数据结构体存储
			FSavedMap NewSaveMap;
			NewSaveMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Emplace(NewSaveMap);
		}

		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty();  //清空存储的所有Actors;

		//使用迭代器，遍历场景里的每一个Actor，将需要保存Actor数据保存到结构体内
		for(FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			//if(!IsValid(Actor) || !Actor->Implements<UISSaveInterface>()) continue;
			if(IsValid(Actor) && Actor->Implements<UISSaveInterface>())
			{
				
				FSavedActor SavedActor;  // 创建以保存场景Actor的结构体对象
				SavedActor.ActorName = Actor->GetFName();
				SavedActor.Transform = Actor->GetTransform();


				//创建一个 FMemoryWriter，用于将数据写入SavedActor.Bytes
				//若 bIsPersistent = true，表示写入的内存数据不会被引擎的垃圾回收（GC）自动释放,反之
				FMemoryWriter MemoryWriter(SavedActor.Bytes,true);

				//创建一个序列化器，将对象的成员以名称和值的形式保存到 MemoryWriter。
				FObjectAndNameAsStringProxyArchive Archive(MemoryWriter,true);
				Archive.ArIsSaveGame = true;//1是保存SaveGame为true的属性，0是全部保存
				Archive.ArNoDelta = true;
			
				Actor->Serialize(Archive);  //序列化Actor的数据

				SavedActor.SaveActorClass = Actor->GetClass();  //存入对象的类模板

				SavedMap.SavedActors.AddUnique(SavedActor);
			}
		}

		for(FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if(MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}

		//保存存档
		UGameplayStatics::SaveGameToSlot(SaveGame,ISGameInstance->LoadSlotName,ISGameInstance->SlotIndex);
	}
}

//加载地图状态
void AISGameplayMode::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix); //从关卡名称这里移除指定前缀，当前为移除通常用于标识流式加载的关卡文件前缀
	
	//获取到游戏实例
	UISGameInstance* ISGameInstance = Cast<UISGameInstance>(GetGameInstance());
	check(ISGameInstance);

	//判断获取的存档是否存在
	if(UGameplayStatics::DoesSaveGameExist(ISGameInstance->LoadSlotName, ISGameInstance->SlotIndex))
	{
		//获取存档
		UISLocalPlayerSaveGame* SaveGame = Cast<UISLocalPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(ISGameInstance->LoadSlotName, ISGameInstance->SlotIndex));
		if(SaveGame == nullptr)
		{
			//存档损坏
		}

		//判断存档是否含有对应关卡的数据
		if(SaveGame->HasMap(WorldName))
		{
			FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);

			for(auto& SaveActors: SavedMap.SavedActors)
			{
				AActor* TargetSpawnActor =  World->SpawnActor<AActor>(SaveActors.SaveActorClass);  //场景中生成Actor
				if(TargetSpawnActor->Implements<UISSaveInterface>())
				{
					//判断当前Actor是否需要设置位置变换
					if(IISSaveInterface::Execute_ShouldLoadTransform(TargetSpawnActor))
					{
						TargetSpawnActor->SetActorTransform(SaveActors.Transform);
						SaveActors.ActorRef = TargetSpawnActor;
					}
				}
			}
			for(auto SaveActors: SavedMap.SavedActors)
			{
				for(FActorIterator It(World); It; ++It)
				{
					AActor* SpawnActor = *It;
					if(SpawnActor == SaveActors.ActorRef)
					{
						//反序列化，创建一个FMemoryReader实例用于从二进制数据中读取内容
						FMemoryReader MemoryReader(SaveActors.Bytes,true);

						//FObjectAndNameAsStringProxyArchive 代理类，用于序列化和反序列化对象的属性 true：表示允许使用字符串形式的对象和属性名称（便于调试和可读性）。
						FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
						Archive.ArIsSaveGame = true; //指定反序列化是用于加载存档数据。1是加载SaveGame为true的属性，0是全部加载
						Archive.ArNoDelta = true;
						SpawnActor->Serialize(Archive); //执行反序列化，将二进制数据设置到actor属性上
					
						//修改Actor上的属性后，调用函数更新Actor的显示
						IISSaveInterface::Execute_LoadActor(SpawnActor);
					}
				}
			}
		}
	}
}
