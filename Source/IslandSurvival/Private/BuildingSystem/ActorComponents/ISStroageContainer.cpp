// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/ActorComponents/ISStroageContainer.h"

void UISStroageContainer::BeginPlay()
{
	if(GetOwner()->HasAuthority())
	{
		InitializeContainerSpace(InventorySpace);
	}
}
