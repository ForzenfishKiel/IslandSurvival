// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISCharacterInventory.h"
void UISCharacterInventory::InitializeBackPackSpace()
{
	for(int32 Index = 0;Index<InventorySpace;Index++)
	{
		InventoryContainer.Add(ItemInfo);  //添加背包空位栏
	}
}
void UISCharacterInventory::BeginPlay()
{
	Super::BeginPlay();
	InitializeBackPackSpace();  //初始化背包
	ItemPickup.AddDynamic(this,&UISItemsContainer::ToPickUpItemsInBackPack);
}
