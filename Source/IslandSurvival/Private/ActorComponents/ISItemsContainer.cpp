// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISItemsContainer.h"
#include "ActorComponents/ISEquipmentComponent.h"
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

void UISItemsContainer::WhenInventoryChange_Implementation(UISItemsContainer* TargetContainer, const int32 TargetIndex)
{
	//若目标位置可用
	if(TargetContainer->InventoryContainer[TargetIndex].ItemID==-1&&IsValid(TargetContainer->InventoryContainer[TargetIndex].ItemClassRef))
	{
		const UISEquipmentComponent*CharacterEquipment = GetOwner()->GetComponentByClass<UISEquipmentComponent>();
		const EItemType ItemType = TargetContainer->InventoryContainer[TargetIndex].ItemType;  //获取对方的物品种类
		if(ItemType==EItemType::Equipable)
		{
			
		}
	}
}

void UISItemsContainer::WhenItemExchanged_Implementation(UISItemsContainer* TargetItemsContainer,const int32 SourceIndex,const int32 TargetIndex)
{
	/*当物品放置到另一个物品的时候的几种情况
	 * 1.当物品可堆叠时，则放置的格子的数量加上对方物品的数量，然后对方物品为空（双方物品ID相同）
	 * 2.当物品不可堆叠时，则无条件交换
	 */
	if(TargetItemsContainer->InventoryContainer[TargetIndex].ItemID==InventoryContainer[SourceIndex].ItemID)
	{
		if(TargetItemsContainer->InventoryContainer[TargetIndex].CanStack&&InventoryContainer[SourceIndex].CanStack)
		{
			InventoryContainer[SourceIndex].ItemQuantity+=TargetItemsContainer->InventoryContainer[TargetIndex].ItemQuantity;
			TargetItemsContainer->InventoryContainer[TargetIndex] = ItemInfo;
			/*更新双方的背包*/
			TargetItemsContainer->InventoryUpdate.Broadcast();
			InventoryUpdate.Broadcast();
			return;
		}
		else
		{
			FItemInformation TempItemInfor = InventoryContainer[SourceIndex];  //暂存
			InventoryContainer[SourceIndex] = TargetItemsContainer->InventoryContainer[TargetIndex];
			TargetItemsContainer->InventoryContainer[TargetIndex] = TempItemInfor;
			/*更新双方背包*/
			TargetItemsContainer->InventoryUpdate.Broadcast();
			InventoryUpdate.Broadcast();
			return;
		}
	}
	else
	{
		FItemInformation TempItemInfor = InventoryContainer[SourceIndex];  //暂存
		InventoryContainer[SourceIndex] = TargetItemsContainer->InventoryContainer[TargetIndex];
		TargetItemsContainer->InventoryContainer[TargetIndex] = TempItemInfor;
		/*更新双方背包*/
		TargetItemsContainer->InventoryUpdate.Broadcast();
		InventoryUpdate.Broadcast();
		return;
	}
}
void UISItemsContainer::InitializeBackPackSpace(const int32 Space)
{
	for(int32 Index = 0;Index<Space;Index++)
	{
		InventoryContainer.Add(ItemInfo);  //添加背包空位栏
	}
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

EContainerType UISItemsContainer::GetTargetContainerType()
{
	return ContainerType;
}


