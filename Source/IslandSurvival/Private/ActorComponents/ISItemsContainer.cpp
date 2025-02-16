// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISItemsContainer.h"
#include "ISInventorySystem.h"
#include "ActorComponents/ISCharacterInventory.h"
#include "ActorComponents/ISEquipmentComponent.h"
#include "ActorComponents/ISHotBarInventory.h"
#include "ActorComponents/ISGearEquipComponent.h"
#include "Engine/ActorChannel.h"
#include "Character/ISCharacter.h"
#include "Game/ISGameInstance.h"
#include "Game/ISGameplayMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UISItemsContainer::UISItemsContainer()
{
	bWantsInitializeComponent = true;  //启用组件的Initialize功能，使其允许复写InitializeComponent
	SetIsReplicatedByDefault(true); //默认打开复制
	PrimaryComponentTick.bCanEverTick = true;
}


void UISItemsContainer::InitializeComponent()
{
	Super::InitializeComponent();
	UISGameInstance* ISGameplayInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(this));
	if(GetOwner()->HasAuthority())
	{
		if(bReplicated)
		{
			auto InventorySystem = UISInventorySystem::CreateInventory(this);  //创建背包对象，并传输背包的属性
			check(InventorySystem);
			RepInventories.Emplace(InventorySystem);
		}
		InitializeContainerSpace(InventorySpace);
	}
}



void UISItemsContainer::WhenInventoryChange(UISItemsContainer* TargetContainer, const int32 TargetIndex)
{
	CurrentChooseIndex = TargetIndex;
	//若目标位置可用
	if(TargetContainer->InventoryContainer[TargetIndex].ItemID!=-1&&IsValid(TargetContainer->InventoryContainer[TargetIndex].ItemClassRef))
	{
		UISEquipmentComponent*CharacterEquipment = GetOwner()->GetComponentByClass<UISEquipmentComponent>();
		UISBuildingComponent*Building = GetOwner()->GetComponentByClass<UISBuildingComponent>();
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
		if(ItemType==EItemType::Consumable)
		{
			CharacterEquipment->UseConsumable(TargetContainer->InventoryContainer[TargetIndex].ItemClassRef);//使用对应的物品
			DiscardItem(TargetIndex,1);  //丢弃/消耗一个物品
			CharacterEquipment->UnUseConsumable();  //停止使用
			return;
		}
		if(ItemType == EItemType::Buildable)
		{
			Building->BuildModeClient(TargetContainer->InventoryContainer[TargetIndex].ItemClassRef,TargetIndex);
			return;
		}
	}
}


void UISItemsContainer::WhenItemExchanged_Implementation(UISItemsContainer* TargetItemsContainer,const int32 SourceIndex,const int32 TargetIndex)
{
	/*当物品放置到另一个物品的时候的几种情况
	 * 1.当物品可堆叠时，则放置的格子的数量加上对方物品的数量，然后对方物品为空（双方物品ID相同）
	 * 2.当物品不可堆叠时，则无条件交换
	 */
	if(!TargetItemsContainer) return;
	if(CheckGearSlotExchanged(TargetItemsContainer,TargetIndex,SourceIndex)) return;  //是否是来自装备的交换
	//相同ID的物品可堆叠与不可堆叠
	if(TargetItemsContainer->InventoryContainer[TargetIndex].ItemID==InventoryContainer[SourceIndex].ItemID)
	{
		if(TargetItemsContainer->InventoryContainer[TargetIndex].CanStack&&InventoryContainer[SourceIndex].CanStack)
		{
			InventoryContainer[SourceIndex].ItemQuantity+=TargetItemsContainer->InventoryContainer[TargetIndex].ItemQuantity;
			TargetItemsContainer->InventoryContainer[TargetIndex] = ItemInfo;
		}
		//不可堆叠
		else
		{
			FItemInformation TempItemInfor = InventoryContainer[SourceIndex];  //暂存
			InventoryContainer[SourceIndex] = TargetItemsContainer->InventoryContainer[TargetIndex];
			TargetItemsContainer->InventoryContainer[TargetIndex] = TempItemInfor;
		}
	}
	else
	{
		FItemInformation TempItemInfor = InventoryContainer[SourceIndex];  //暂存
		InventoryContainer[SourceIndex] = TargetItemsContainer->InventoryContainer[TargetIndex];
		TargetItemsContainer->InventoryContainer[TargetIndex] = TempItemInfor;
	}
	InventoryUpdate.Broadcast();
	TargetItemsContainer->InventoryUpdate.Broadcast();
}

bool UISItemsContainer::CheckGearSlotExchanged(UISItemsContainer* TargetGear, const int32 TargetIndex,const int32 SourceIndex)
{
	//两个装备栏不做交换
	if(Cast<UISGearEquipComponent>(TargetGear)&&GetClass()==UISGearEquipComponent::StaticClass()) return true;
	AISCharacter* SourceCharacter = Cast<AISCharacter>(GetOwner());
	if(!SourceCharacter) return false;
	UISEquipmentComponent*EquipmentComponent = SourceCharacter->GetComponentByClass<UISEquipmentComponent>();
	//如果拖动源是来自装备栏
	if(UISGearEquipComponent* GearEquipComponent = Cast<UISGearEquipComponent>(TargetGear))
	{
		//装备交换
		if(InventoryContainer[SourceIndex].ArmorType!=EArmorType::None && InventoryContainer[SourceIndex].ItemID != -1)  //如果放置的位置是某一个装备
		{
			if(GearEquipComponent->InventoryContainer[TargetIndex].ArmorType != InventoryContainer[SourceIndex].ArmorType) return true;  //两个装备交换必须是同物品的
			
			FItemInformation Information = GearEquipComponent->InventoryContainer[TargetIndex];  //获取拖动源位置的物品信息
			
			EquipmentComponent->OnUnEquipGear.Broadcast(GearEquipComponent->InventoryContainer[TargetIndex]);  //广播卸下装备
			
			EquipmentComponent->OnEquipGear.Broadcast(GearEquipComponent->InventoryContainer[TargetIndex]);//广播穿上装备

			GearEquipComponent->InventoryContainer[TargetIndex] = InventoryContainer[SourceIndex]; //装备栏装备交换的物品信息
			
			InventoryContainer[SourceIndex] = Information;//放置位置进行交换

			TargetGear->InventoryUpdate.Broadcast();
			InventoryUpdate.Broadcast();
			
			return true;
		}
		else if(InventoryContainer[SourceIndex].ItemID==-1)  //如果放置源位置是空
		{
			
			FItemInformation TargetGearSlot = GearEquipComponent->InventoryContainer[TargetIndex];  //找到这个装备插槽里的装备
			
			FItemInformation NewItemInfo;
			NewItemInfo.ArmorType = GearEquipComponent->InventoryContainer[TargetIndex].ArmorType;
			
			InventoryContainer[SourceIndex] = GearEquipComponent->InventoryContainer[TargetIndex];  //解引用并储存数据
			
			EquipmentComponent->OnUnEquipGear.Broadcast(TargetGearSlot);  //广播脱下装备
			
			GearEquipComponent->InventoryContainer[TargetIndex] = NewItemInfo;
			
			TargetGear->InventoryUpdate.Broadcast();
			InventoryUpdate.Broadcast();

			return true;  //跳过当前执行
		}
		else
		{
			return true;
		}
	}
	//如果拖动源是来自其他库存，放置源是装备栏
	else if(GetClass()==UISGearEquipComponent::StaticClass())
	{
		if(TargetGear->InventoryContainer[TargetIndex].ArmorType!=EArmorType::None)
		{
			if(InventoryContainer[SourceIndex].ArmorType != TargetGear->InventoryContainer[TargetIndex].ArmorType) return true;
			
			FItemInformation Information = InventoryContainer[SourceIndex];
			
			EquipmentComponent->OnUnEquipGear.Broadcast(InventoryContainer[SourceIndex]);  //广播卸下装备
			
			EquipmentComponent->OnEquipGear.Broadcast(TargetGear->InventoryContainer[TargetIndex]);  //广播穿上装备

			InventoryContainer[SourceIndex] = TargetGear->InventoryContainer[TargetIndex];
			
			TargetGear->InventoryContainer[TargetIndex] = Information;  //目标位置等于被替换的装备位置

			TargetGear->InventoryUpdate.Broadcast();
			InventoryUpdate.Broadcast();
			
			
			return true;
		}
		else if(InventoryContainer[SourceIndex].ItemID == -1)
		{
			if(InventoryContainer[SourceIndex].ArmorType != TargetGear->InventoryContainer[TargetIndex].ArmorType) return true;
			
			FItemInformation TargetGearSlot = TargetGear->InventoryContainer[TargetIndex];

			EquipmentComponent->OnEquipGear.Broadcast(TargetGear->InventoryContainer[TargetIndex]);  //广播穿上装备
			
			InventoryContainer[SourceIndex] = TargetGearSlot;

			TargetGear->InventoryContainer[TargetIndex] = ItemInfo;

			TargetGear->InventoryUpdate.Broadcast();
			InventoryUpdate.Broadcast();

			return true;
		}
	}
	return false;
}

//丢弃/销毁物品
void UISItemsContainer::DiscardItem_Implementation(const int32 TargetIndex, const int32 TargetQuantity)
{
	if(TargetIndex>=0&&TargetQuantity>=1)
	{
		InventoryContainer[TargetIndex].ItemQuantity-=TargetQuantity;
		if(InventoryContainer[TargetIndex].ItemQuantity==0)
		{
			InventoryContainer[TargetIndex] = ItemInfo;
		}
		InventoryUpdate.Broadcast();
	}
}


void UISItemsContainer::DiscardItemFromID_Implementation(const int32 TargetID, const int32 TargetQuantity)
{
	for(auto& ContainerRef : InventoryContainer)
	{
		if(ContainerRef.ItemID==TargetID)
		{
			ContainerRef.ItemQuantity -= TargetQuantity;
			if(ContainerRef.ItemQuantity==0)
			{
				ContainerRef = ItemInfo;
			}
			InventoryUpdate.Broadcast();
			return;
		}
	}
}


void UISItemsContainer::OnRep_ContainerChange()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("本地数组更新变化"));
	InventoryUpdate.Broadcast();
}

void UISItemsContainer::InitializeContainerSpace(const int32 Space)
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


void UISItemsContainer::ToPickUpItemsInBackPack_Implementation(const FItemInformation Information, const int32 TargetQuantity)
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
				if(TargetQuantity >= 1)
				{
					InventoryContainer[index].ItemQuantity += TargetQuantity;
					return;
				}
				InventoryContainer[index].ItemQuantity+= Information.ItemQuantity;  //物品数量相加
				return;
			}
		}
		for(int32 index = 0; index < InventoryContainer.Num(); index++)
		{
			if(InventoryContainer[index].ItemID==-1)
			{
				InventoryContainer[index] = Information;
				if(TargetQuantity >= 1)
				{
					InventoryContainer[index].ItemQuantity = TargetQuantity;
					return;
				}
				InventoryContainer[index].ItemQuantity = Information.ItemQuantity;
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
				if(TargetQuantity >= 1)
				{
					InventoryContainer[index].ItemQuantity = TargetQuantity;
					return;
				}
				InventoryContainer[index].ItemQuantity = Information.ItemQuantity;
				return;
			}
		}
	}
}
// Called when the game starts
void UISItemsContainer::BeginPlay()
{
	Super::BeginPlay();
	ItemDiscard.AddDynamic(this,&UISItemsContainer::DiscardItem);
}

// Called every frame
void UISItemsContainer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}



/*  NetWorking  */
void UISItemsContainer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

			
	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(UISItemsContainer,InventoryContainer,SharedParams);
}

bool UISItemsContainer::CheckConnection(UActorChannel* Channel)
{
	if(Channel&&Channel->Connection && Channel->Connection->OwningActor)
	{
		return RepInventoryPlayer.Contains(Channel->Connection->OwningActor);
	}
	return false;
}

bool UISItemsContainer::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool Result = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	if(CheckConnection(Channel))
	{
		for(auto& Inventory: RepInventories)  //输送用于网络复制的数组
		{
			Result |= Channel->ReplicateSubobject(Inventory,*Bunch,*RepFlags);  //传输所有复制的成员
		}
	}
	return Result;
}
/*Ending NetWork*/


EContainerType UISItemsContainer::GetTargetContainerType()
{
	return ContainerType;
}

FName UISItemsContainer::GetContainerName()
{
	return ContainerName;
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
			TargetHotBar->ItemPickup.Broadcast(TargetItemInfo,0);
			ItemPickupOnUI.Broadcast(TargetItemInfo);
			TargetHotBar->InventoryUpdate.Broadcast();
			FString ItemNameToPrint = FString::Printf(TEXT("已拾取: %s"), *UserInfo->ItemName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *ItemNameToPrint);
			return;
		}
		//物品加进角色背包
		if(TargetInventory->CheckInventoryEmpty(TargetItemInfo))
		{
			TargetInventory->ItemPickup.Broadcast(TargetItemInfo,0);
			ItemPickupOnUI.Broadcast(TargetItemInfo);
			TargetInventory->InventoryUpdate.Broadcast();
			FString ItemNameToPrint = FString::Printf(TEXT("已拾取: %s"), *UserInfo->ItemName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *ItemNameToPrint);
			return;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("背包已满！！"));
	}
}

UISInventorySystem* UISItemsContainer::FindInventoryByName(const FName InName)
{
	for(auto InventoryRef : RepInventories)
	{
		if(InventoryRef->InventoryName == InName)
		{
			return InventoryRef;
		}
	}
	return nullptr;
}