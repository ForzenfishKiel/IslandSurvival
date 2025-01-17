// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ISCollectbleDataAsset.h"

TSubclassOf<AISHarvestingBase> UISCollectbleDataAsset::GetHavestingBaseClass(const FString InName)
{
	return ISCollectbleStaticMeshMap.FindChecked(InName);
}
