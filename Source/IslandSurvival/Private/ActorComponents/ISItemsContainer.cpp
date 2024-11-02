// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISItemsContainer.h"

#include "ActorComponents/ISCharacterInventory.h"
#include "ActorComponents/ISEquipmentComponent.h"
#include "ActorComponents/ISHotBarInventory.h"
#include "Character/ISCharacter.h"
#include "Game/ISGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UISItemsContainer::UISItemsContainer()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UISItemsContainer::WhenInventoryChange(UISItemsContainer* TargetContainer, const int32 TargetIndex)
{
	//若目标位置可用
	if(TargetContainer->InventoryContainer[TargetIndex].ItemID!=-1&&IsValid(TargetContainer->InventoryContainer[TargetIndex].ItemClassRef))
	{
		UISEquipmentComponent*CharacterEquipment = GetOwner()->GetComponentByClass<UISEquipmentComponent>();
		const EItemType ItemType = TargetContainer->InventoryContainer[TargetIndex].ItemType;  //获取对方的物品种类
		if(ItemType==EItemType::Equipable)
		{
			if(CharacterEquipment->Equipable==nullptr)  //当前处于未装备
			{
				if(LastChooseIndex==-1)
				{
					LastChooseIndex=TargetIndex;  //记录当前的Index
					CharacterEquipment->OnEquip.Broadcast(TargetContainer->InventoryContainer[TargetIndex]);
					return;
				}
			}
			else
			{
				if(LastChooseIndex!=TargetIndex)
				{
					LastChooseIndex=TargetIndex;
					CharacterEquipment->OnUnEquip.Broadcast();  //删除当前装备的武器
					CharacterEquipment->OnEquip.Broadcast(TargetContainer->InventoryContainer[TargetIndex]);  //然后装上新的武器
					return;
				}
				if(LastChooseIndex==TargetIndex)
				{
					LastChooseIndex=-1;
					CharacterEquipment->OnUnEquip.Broadcast();
				}
				return;
			}
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

bool UISItemsContainer::CheckInventoryEmpty(const FItemInformation Information)
{
	for(int32 Index = 0;Index<InventoryContainer.Num();Index++)
	{
		if(InventoryContainer[Index].ItemID==Information.ItemID&&InventoryContainer[Index].CanStack)
		{
			return true;  //找到可堆叠
		}
		if(InventoryContainer[Index].ItemID==-1)
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

void UISItemsContainer::PickUpItemForActor(APawn* TargetPawn, AActor* TargetActor)
{
	AISItemBase*SourceItem = Cast<AISItemBase>(TargetActor);
	FName TargetItemID = SourceItem->ItemID;
	PickUpItemForID(TargetPawn,TargetItemID,1);
}

//客户端运行
void UISItemsContainer::PickUpItemForID_Implementation(APawn* TargetPawn, FName TargetID, const int32 TargetNums)
{
	UISGameInstance* TargetGameInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!TargetGameInstance&&TargetNums<1) return;
	const UDataTable*UserDT = TargetGameInstance->ItemDataTable;
	check(UserDT);
	const FItemInformation*UserInfo = UserDT->FindRow<FItemInformation>(TargetID,TEXT("name"));
	if (UserInfo)
	{
		FItemInformation TargetItemInfo = *UserInfo;
		TargetItemInfo.ItemQuantity+=TargetNums - 1;  //数量相加
		AISCharacter*SourceCharacter = Cast<AISCharacter>(TargetPawn);
		if(!SourceCharacter) return;
		UISCharacterInventory*TargetInventory = SourceCharacter->CharacterInventory;
		UISHotBarInventory*TargetHotBar = SourceCharacter->CharacterHotBarInventory;
		if(!TargetInventory&&!TargetHotBar) return;
		//物品添加进物品栏
		if(TargetHotBar->CheckInventoryEmpty(TargetItemInfo))
		{
			TargetHotBar->ItemPickup.Broadcast(TargetItemInfo);
			ItemPickupOnUI.Broadcast(TargetItemInfo);
			TargetHotBar->InventoryUpdate.Broadcast();
			FString ItemNameToPrint = FString::Printf(TEXT("已拾取: %s"), *UserInfo->ItemName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *ItemNameToPrint);
			return;
		}
		//物品加进角色背包
		if(TargetInventory->CheckInventoryEmpty(TargetItemInfo))
		{
			TargetInventory->ItemPickup.Broadcast(TargetItemInfo);
			ItemPickupOnUI.Broadcast(TargetItemInfo);
			TargetInventory->InventoryUpdate.Broadcast();
			FString ItemNameToPrint = FString::Printf(TEXT("已拾取: %s"), *UserInfo->ItemName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *ItemNameToPrint);
			return;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("背包已满！！"));
	}
}


