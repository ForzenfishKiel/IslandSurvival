// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISCharacterInventory.h"
void UISCharacterInventory::BeginPlay()
{
	Super::BeginPlay();
	ItemPickup.AddDynamic(this,&UISItemsContainer::ToPickUpItemsInBackPack);
}
