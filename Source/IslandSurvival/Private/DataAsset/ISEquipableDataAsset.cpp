// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ISEquipableDataAsset.h"

FEquipItemRarityContainer UISEquipableDataAsset::GetTargetItemRarityContainer(FName TargetID)
{
	return EquipRarityMap.FindChecked(TargetID);
}
