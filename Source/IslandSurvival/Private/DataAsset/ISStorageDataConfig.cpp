// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ISStorageDataConfig.h"

FStorageDataConfig* UISStorageDataConfig::GetInventoryConfig(EContainerType ContainerType)
{
	return InventoryConfigMap.Find(ContainerType);
}
