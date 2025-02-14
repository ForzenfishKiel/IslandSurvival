// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSave/ISLocalPlayerSaveGame.h"

FSavedMap UISLocalPlayerSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	//遍历保存的地图数据
	for(const FSavedMap& Map : SavedMaps)
	{
		if(Map.MapAssetName == InMapName)
		{
			return Map;
		}
	}
	return FSavedMap();
}

bool UISLocalPlayerSaveGame::HasMap(const FString& InMapName)
{
	for(const FSavedMap& Map : SavedMaps)
	{
		if(Map.MapAssetName == InMapName)
		{
			return true;
		}
	}
	return false;
}
