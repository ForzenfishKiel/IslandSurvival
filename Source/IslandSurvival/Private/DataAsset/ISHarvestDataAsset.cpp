// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ISHarvestDataAsset.h"

TSubclassOf<AISHarvestingBase> UISHarvestDataAsset::GetHarvest(const FString& InName) const
{
	for(auto Iter = StaticMeshMap.CreateConstIterator(); Iter; ++Iter)
	{
		if(InName.Contains(Iter->Key))
		{
			return StaticMeshMap.FindChecked(Iter->Key);
		}
	}
	return nullptr;
}
