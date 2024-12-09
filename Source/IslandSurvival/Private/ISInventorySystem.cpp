// Fill out your copyright notice in the Description page of Project Settings.


#include "ISInventorySystem.h"
#include "ActorComponents/ISItemsContainer.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Data/ISPlayerItemDataTable.h"
#include "Engine/ActorChannel.h"
#include "Game/ISGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

//根据全局唯一标识符查询指定的类引用地址
UISPlayerItemDataTable* UISInventorySystem::GetItemNameFormGuid(const FGuid& InGuid)
{
	if(auto Result = ItemMap.Find(InGuid)) return *Result;
	return nullptr;
}

//从传入的ID获取指定的物品数量
int32 UISInventorySystem::GetItemCountByName(const FName& InName)
{
	int32 Count = 0;
	for(auto Item : Items)
	{
		if(Item->GetName()==InName)
		{
			Count += Item->GetItemCount();
		}
	}
	return Count;
}

//背包物品添加逻辑
bool UISInventorySystem::AddItems(const TArray<FItemInformation>& InItems)
{
	TMap<FName,int32> CountMap;//根据ID获取对应数量的临时Map储存器
	//查询整个外层背包，将外层数据背包依次导入进行处理
	for(auto& Info:InItems)
	{
		// 查找 ItemName 是否已经存在，如果不存在则添加并初始化为 0，再进行数量累加
		int32 MyInt = Info.ItemID;
		FString IntAsString = FString::Printf(TEXT("%d"), MyInt);  // 将 int32 转换为 FString
		CountMap.FindOrAdd(FName(*IntAsString),0)+=Info.ItemQuantity;
	}
	//查询整个内层背包，优先添加可堆叠的物品
	for(auto Item:Items)
	{
		if(auto Result = CountMap.Find(Item->GetItemName()))
		{
			if(*Result == Item->GetItemCount()) continue;  //如果数据一样则跳过当前检索
			int32& Count = *Result;  //引用数据，更改后会直接更改Map的Value
			if(auto Row =UISAbilitysystemLibary::FindTargetItemData(OwnerComponent->GetOwner(),Item->GetItemName()))
			{
				int32 AddCount  = FMath::Min(Row->MaxStack - Item->GetItemCount(),Count);  /*
				最大物品值减去现有物品值（如果外层数据和内层数据物品总数完全一样则为0也就是不添加）
				这是AddCount取最小值的意义*/
				Item->SetCount(Item->GetItemCount() + AddCount);  //更改内层数据
				Count -= AddCount;  //算出实际物品数量
				if(Count<=0)
				{
					//从数量表中删除成员
					CountMap.Remove(Item->GetItemName());  //整个结构确保Key是唯一对应的
				}
			}
		}
	}

	
	//可堆叠道具已经全部堆叠完毕，剩余道具则只需要创建新道具（该情况适用于在超出堆叠上限的物品添加时，多出来的部分新开一个数据地址进行相加）
	if(CountMap.Num()>0)
	{
		if(CountMap.Num()>Size - Items.Num()) return false;
		for(auto Iter = CountMap.CreateIterator();Iter;++Iter)  //获取Map的总长度
		{
			if(auto Row =UISAbilitysystemLibary::FindTargetItemData(OwnerComponent->GetOwner(),Iter->Key))
			{
				while(Iter->Value > 0)
				{
					auto Item = AddItem_Internal();
					Item->SetItemName(Iter->Key);
					Item->SetCount(FMath::Min(Row->MaxStack , Iter->Value));
					Item->OnRep_OwnerInventory();
					Iter->Value -= Row->MaxStack;
				}
				Iter.RemoveCurrent();  //移除当前键值对，也就是删除一个成员
			}
		}
	}
	return CountMap.Num()<=0;
}


UISPlayerItemDataTable* UISInventorySystem::AddItem_Internal()
{
	auto Item = NewObject<UISPlayerItemDataTable>(GetOuter());
	Item->SetGuid(FGuid::NewGuid());
	Item->SetOwnerInventory(this);
	Items.Add(Item);
	ItemMap.Add(Item->GetGuid(),Item);
	return Item;  //返回并添加内层数据
}

bool UISInventorySystem::RemoveItems(const TArray<FItemInformation>& InItemsInfo)
{
	TMap<FName,int32> CountMap;
	for(auto& Info:InItemsInfo)
	{
		CountMap.FindOrAdd(Info.ItemName,0)+=Info.ItemQuantity;
	}

	for(auto Item:Items)
	{
		if(auto Result = CountMap.Find(Item->GetItemName()))
		{
			int32& Count = *Result;
			int32 MinusCount = FMath::Min(Item->GetItemCount(),Count);
			Item->SetCount(Item->GetItemCount() - MinusCount);
			if(Item->GetItemCount() <= 0)
			{
				RemoveItem_Internal(Item);  //移除
			}
			Count -= MinusCount;
			if (Count <= 0)
			{
				CountMap.Remove(Item->GetItemName());
			}
		}
	}
	return CountMap.Num()<=0;
}

//在服务器中移除指定物品数据
void UISInventorySystem::RemoveItem_Internal(UISPlayerItemDataTable* InItem)
{
	Items.RemoveSwap(InItem);
	ItemMap.Remove(InItem->GetGuid());
	InItem->MarkAsGarbage();  //当成垃圾回收
}

//根据唯一指定标识符删除指定数据地址
bool UISInventorySystem::RemoveItemByGuid(const FGuid& InGuid, int32 Count)
{
	if(auto Result = ItemMap.Find(InGuid))
	{
		auto Item = *Result;
		if(Item->GetItemCount() < Count) return false;
		Item->SetCount(Item->GetItemCount() - Count);
		if(Item->GetItemCount() <= 0)
		{
			RemoveItem_Internal(Item);
		}
		return true;
	}
	return false;
}

void UISInventorySystem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UISInventorySystem,OwnerComponent);
	DOREPLIFETIME(UISInventorySystem,Tag);
	DOREPLIFETIME(UISInventorySystem,Size);
}
/*
 * 该函数用UActorChannel来Replicate在UInventory中所有UItem，当该Channel所属Actor被同步到客户端时，
 * 这UItem也会作为Actor的Subobject同步到了客户端（UActorComponent的同步也是这个原理，相关代码可查阅UE源码AActor::ReplicateSubobjects函数）。
 */
bool UISInventorySystem::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool Result = false;
	if(Channel)
	{
		for(auto Item:Items)
		{
			Result |= Channel->ReplicateSubobject(Item,*Bunch,*RepFlags);  //传输数据到客户端
		}
	}
	return Result;
}

void UISInventorySystem::ClientRegisterReplicatedItem(UISPlayerItemDataTable* InItem)
{
	if(IsValid(InItem) && !ItemMap.Contains(InItem->GetGuid()))
	{
		Items.Add(InItem);
		ItemMap.Add(InItem->GetGuid(),InItem);
	}
}

void UISInventorySystem::ClientUnRegisterReplicatedItem(UISPlayerItemDataTable* InItem)
{
	if(IsValid(InItem))
	{
		Items.Remove(InItem);
		ItemMap.Remove(InItem->GetGuid());
	}
}

//内层背包数据变化通知
void UISInventorySystem::MarkItemChange()
{
	TArray<FItemInformation> ItemsInfo;
	for(auto Item:Items)
	{
		FItemInformation ItemInfo;
	}
}

//创建并返回自身实例，创建配置
UISInventorySystem* UISInventorySystem::CreateInventory(UISItemsContainer* InOwnerComponent, const FInventoryConfig InventoryConfig)
{
	UISInventorySystem* NewInventory = NewObject<UISInventorySystem>();
	NewInventory->OwnerComponent = InOwnerComponent;
	NewInventory->Size = InventoryConfig.InventorySpace;
	NewInventory->Tag = InventoryConfig.ContainerName;
	return NewInventory;
}
