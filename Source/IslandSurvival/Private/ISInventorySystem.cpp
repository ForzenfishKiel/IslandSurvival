// Fill out your copyright notice in the Description page of Project Settings.


#include "ISInventorySystem.h"
#include "Net/UnrealNetwork.h"


void UISInventorySystem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UISInventorySystem,InternetInventory,SharedParams);  //将用于网络复制传输的成员变量标记为脏
	DOREPLIFETIME_WITH_PARAMS_FAST(UISInventorySystem,Size,SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UISInventorySystem,InventoryName,SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UISInventorySystem,OwnerComponent,SharedParams);
}

UISInventorySystem* UISInventorySystem::CreateInventory(UISItemsContainer* InOwnerContainer)
{
	UISInventorySystem* NewInventory = NewObject<UISInventorySystem>();

	NewInventory->OwnerComponent = InOwnerContainer;
	NewInventory->Size = InOwnerContainer->InventorySpace;
	NewInventory->InventoryName = InOwnerContainer->ContainerName;
	return NewInventory;
}
