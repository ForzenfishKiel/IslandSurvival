// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISHotBarInventory.h"
#include "Net/UnrealNetwork.h"
void UISHotBarInventory::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwner()->HasAuthority())
	{
		InitializeContainerSpace(InventorySpace);
	}
	ItemPickup.AddDynamic(this,&UISItemsContainer::ToPickUpItemsInBackPack);
}
