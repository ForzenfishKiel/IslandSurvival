// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ISCollectibleDataAsset.h"

FDropInformation UISCollectibleDataAsset::GetDropConfig(ECollectibleClass TargetClass)
{
	return DropConfigs.FindChecked(TargetClass);
}
