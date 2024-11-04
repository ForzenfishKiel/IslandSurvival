// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ISCraftingContainer.h"

FCraftingArray UISCraftingContainer::GetCraftingInfoMap(EContainerType WorkSpaceType)
{
	return CraftingInfoMap.FindChecked(WorkSpaceType);
}
