// Fill out your copyright notice in the Description page of Project Settings.
#include "Data/ISPlayerItemDataTable.h"

#include "Game/ISGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"



void UISPlayerItemDataTable::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UISPlayerItemDataTable,Guid,SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UISPlayerItemDataTable,ItemName,SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UISPlayerItemDataTable,ItemCount,SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UISPlayerItemDataTable,OwnerInventory,SharedParams);
}

//Getter
FName UISPlayerItemDataTable::GetItemName() const
{
	return ItemName;
}

int32 UISPlayerItemDataTable::GetItemCount() const
{
	return ItemCount;
}

FGuid UISPlayerItemDataTable::GetGuid() const
{
	return Guid;
}

//Setter
void UISPlayerItemDataTable::SetGuid(const FGuid& InGuid)
{
	Guid = InGuid;
	MARK_PROPERTY_DIRTY_FROM_NAME(UISPlayerItemDataTable,Guid,this);
}

void UISPlayerItemDataTable::SetCount(int32 InCount)
{
	ItemCount = InCount;
	MARK_PROPERTY_DIRTY_FROM_NAME(UISPlayerItemDataTable,ItemCount,this);
}

void UISPlayerItemDataTable::SetOwnerInventory(UISInventorySystem* InInventory)
{
	OwnerInventory = InInventory;
	MARK_PROPERTY_DIRTY_FROM_NAME(UISPlayerItemDataTable,OwnerInventory,this);
}

void UISPlayerItemDataTable::SetItemName(FName InName)
{
	ItemName = InName;
	MARK_PROPERTY_DIRTY_FROM_NAME(UISPlayerItemDataTable,ItemName,this);
}

void UISPlayerItemDataTable::OnRep_OwnerInventory()
{
	if(IsValid(OwnerInventory))
	{
		OwnerInventory->ClientRegisterReplicatedItem(this);
	}
}

void UISPlayerItemDataTable::PreDestoryFromReplication()
{
	if(IsValid(OwnerInventory))
	{
		OwnerInventory->ClientUnRegisterReplicatedItem(this);
	}
}
