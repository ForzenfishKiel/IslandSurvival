// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISHotBarInventory.h"
#include "Net/UnrealNetwork.h"
void UISHotBarInventory::BeginPlay()
{
	Super::BeginPlay();
	ItemPickup.AddDynamic(this,&UISItemsContainer::ToPickUpItemsInBackPack);
}

void UISHotBarInventory::UpdateAmmos(const int32 InCurrentAmmo, const int32 InMaxAmmo)
{
	OnInventoryUpdateCurrentAmmos.Broadcast(InCurrentAmmo,InMaxAmmo,CurrentChooseIndex);
}
