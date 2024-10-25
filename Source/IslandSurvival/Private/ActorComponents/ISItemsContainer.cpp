// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISItemsContainer.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UISItemsContainer::UISItemsContainer()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UISItemsContainer::CheckEmptySlotInInventory(const TArray<FItemInformation>&TargetInventory)
{
	for (FItemInformation Item : TargetInventory)
	{
		if(Item.ItemID==-1)
		{
			return true;
		}
	}
	return false;
}

void UISItemsContainer::ToPickUpItemsInBackPack_Implementation(const FItemInformation Information)
{
	/*物品拾取的一些情况
* 优先搜索背包里含有的相同物品
* 1.如果物品可堆叠，则该背包的位置的物品数量+1
* 2.如果物品不可堆叠，则寻找一个空位放置
* 如果没有相同的物品，则搜索背包内的空位进行防止
* 以上情况都不满足，只能是背包爆满
*/
	if(InventoryContainer.IsEmpty()) {return;} //库存器错误
	if(Information.CanStack == true)
	{
		for(int32 index = 0; index < InventoryContainer.Num(); index++)
		{
			if(InventoryContainer[index].ItemID==Information.ItemID)
			{
				InventoryContainer[index].ItemQuantity+=Information.ItemQuantity;  //物品数量相加
				return;
			}
		}
		for(int32 index = 0; index < InventoryContainer.Num(); index++)
		{
			if(InventoryContainer[index].ItemID==-1)
			{
				InventoryContainer[index] = Information;
				return;
			}
		}
	}
	else
	{
		for(int32 index = 0; index < InventoryContainer.Num(); index++)
		{
			if(InventoryContainer[index].ItemID==-1)
			{
				InventoryContainer[index] = Information;
				return;
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("背包已满！！"));
}

// Called when the game starts
void UISItemsContainer::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UISItemsContainer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UISItemsContainer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UISItemsContainer,InventoryContainer);
}

