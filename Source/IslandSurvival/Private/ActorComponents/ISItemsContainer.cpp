// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISItemsContainer.h"
#include "ActorComponents/ISCharacterInventory.h"
#include "ActorComponents/ISEquipmentComponent.h"
#include "ActorComponents/ISHotBarInventory.h"
#include "ActorComponents/ISGearEquipComponent.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Character/ISCharacter.h"
#include "Engine/ActorChannel.h"
#include "Game/ISGameInstance.h"
#include "Game/ISPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UISItemsContainer::UISItemsContainer()
{
	PrimaryComponentTick.bCanEverTick = true;
}



//当物品发生变化
void UISItemsContainer::WhenInventoryChange(UISItemsContainer* TargetContainer, const int32 TargetIndex)
{
	//若目标位置可用
	if(TargetContainer->InventoryContainer[TargetIndex].ItemID!=-1&&IsValid(TargetContainer->InventoryContainer[TargetIndex].ItemClassRef))
	{
		UISEquipmentComponent* CharacterEquipment = GetOwner()->GetComponentByClass<UISEquipmentComponent>();
		UISBuildingComponent* Building = GetOwner()->GetComponentByClass<UISBuildingComponent>();
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

//当物品发生交换
void UISItemsContainer::WhenItemExchanged_Implementation(UISItemsContainer* TargetItemsContainer,const int32 SourceIndex,const int32 TargetIndex)
{
	/*当物品放置到另一个物品的时候的几种情况
	 * 1.当物品可堆叠时，则放置的格子的数量加上对方物品的数量，然后对方物品为空（双方物品ID相同）
	 * 2.当物品不可堆叠时，则无条件交换
	 */
	if(CheckGearSlotEcchanged(TargetItemsContainer,TargetIndex,SourceIndex)) return;  //是否是来自装备的交换
	if(TargetItemsContainer->InventoryContainer[TargetIndex].ItemID==InventoryContainer[SourceIndex].ItemID)
	{
		if(TargetItemsContainer->InventoryContainer[TargetIndex].CanStack&&InventoryContainer[SourceIndex].CanStack)
		{
			InventoryContainer[SourceIndex].ItemQuantity+=TargetItemsContainer->InventoryContainer[TargetIndex].ItemQuantity;
			TargetItemsContainer->InventoryContainer[TargetIndex] = ItemInfo;
			/*更新双方的背包*/
			CallContainerUpdateOnClient(TargetItemsContainer);
			CallContainerUpdateOnClient(this);
			return;
		}
		//不可堆叠
		else
		{
			FItemInformation TempItemInfor = InventoryContainer[SourceIndex];  //暂存
			InventoryContainer[SourceIndex] = TargetItemsContainer->InventoryContainer[TargetIndex];
			TargetItemsContainer->InventoryContainer[TargetIndex] = TempItemInfor;
			/*更新双方背包*/
			CallContainerUpdateOnClient(TargetItemsContainer);
			CallContainerUpdateOnClient(this);
			return;
		}
	}
	else
	{
		FItemInformation TempItemInfor = InventoryContainer[SourceIndex];  //暂存
		InventoryContainer[SourceIndex] = TargetItemsContainer->InventoryContainer[TargetIndex];
		TargetItemsContainer->InventoryContainer[TargetIndex] = TempItemInfor;
		/*更新双方背包*/
		CallContainerUpdateOnClient(TargetItemsContainer);
		CallContainerUpdateOnClient(this);
		return;
	}
}


bool UISItemsContainer::CheckGearSlotEcchanged(UISItemsContainer* TargetGear, const int32 TargetIndex,const int32 SourceIndex)
{
	//两个装备栏不做交换
	if(Cast<UISGearEquipComponent>(TargetGear)&&GetClass()==UISGearEquipComponent::StaticClass()) return true;
	AISCharacter* SourceCharacter = Cast<AISCharacter>(GetOwner());
	if(!SourceCharacter) return false;
	UISEquipmentComponent* EquipmentComponent = SourceCharacter->GetComponentByClass<UISEquipmentComponent>();
	//如果拖动源是来自装备栏
	if(UISGearEquipComponent* GearEquipComponent = Cast<UISGearEquipComponent>(TargetGear))
	{
		//装备交换
		if(InventoryContainer[SourceIndex].ArmorType!=EArmorType::None)  //如果放置的位置是某一个装备
		{
			TArray<EArmorType> MapArray;
			GearEquipComponent->GearEquipContainer.GenerateKeyArray(MapArray);  //将Map里的键用数组储存
			FItemInformation Information = GearEquipComponent->GearEquipContainer[InventoryContainer[SourceIndex].ArmorType];  //获取放置位的物品信息
			EquipmentComponent->OnUnEquipGear.Broadcast(*GearEquipComponent->GearEquipContainer.Find(MapArray[TargetIndex]));  //广播卸下装备
			GearEquipComponent->GearEquipContainer[InventoryContainer[SourceIndex].ArmorType] = InventoryContainer[SourceIndex]; //装备栏装备交换的物品信息
			EquipmentComponent->OnEquipGear.Broadcast(*GearEquipComponent->GearEquipContainer.Find(InventoryContainer[SourceIndex].ArmorType));//广播穿上装备
			InventoryContainer[SourceIndex] = Information;//放置位置进行交换
			TargetGear->InventoryUpdate.Broadcast(); //更新双方库存
			InventoryUpdate.Broadcast();  //除了装备以外的库存
			return true;
		}
		else if(InventoryContainer[SourceIndex].ItemID==-1)
		{
			TArray<EArmorType> MapArray;
			GearEquipComponent->GearEquipContainer.GenerateKeyArray(MapArray);  //将Map里的键用数组储存
			FItemInformation*TargetGearSlot = GearEquipComponent->GearEquipContainer.Find(MapArray[TargetIndex]);  //找到这个装备插槽里的装备
			if(TargetGearSlot == nullptr) return true;  //数据异常，跳过当前执行
			InventoryContainer[SourceIndex] =*TargetGearSlot;  //解引用并储存数据
			EquipmentComponent->OnUnEquipGear.Broadcast(*TargetGearSlot);  //广播脱下装备
			GearEquipComponent->GearEquipContainer[MapArray[TargetIndex]] = ItemInfo;  //清空装备槽显示
			//更新双方背包
			TargetGear->InventoryUpdate.Broadcast();
			InventoryUpdate.Broadcast();
			return true;  //跳过当前执行
		}
		else
		{
			return true;
		}
	}
	//如果拖动源是来自其他库存
	else if(GetClass()==UISGearEquipComponent::StaticClass())
	{
		if(TargetGear->InventoryContainer[TargetIndex].ArmorType!=EArmorType::None)
		{
			TArray<EArmorType> MapArray;
			GearEquipContainer.GenerateKeyArray(MapArray);  //将Map里的键用数组储存
			FItemInformation Information = GearEquipContainer[TargetGear->InventoryContainer[TargetIndex].ArmorType];
			EquipmentComponent->OnUnEquipGear.Broadcast(*GearEquipContainer.Find(MapArray[SourceIndex]));  //广播卸下装备
			GearEquipContainer[TargetGear->InventoryContainer[TargetIndex].ArmorType] = TargetGear->InventoryContainer[TargetIndex];
			EquipmentComponent->OnEquipGear.Broadcast(GearEquipContainer[TargetGear->InventoryContainer[TargetIndex].ArmorType]);  //广播穿上装备
			TargetGear->InventoryContainer[TargetIndex] = Information;  //目标位置等于被替换的装备位置
			TargetGear->InventoryUpdate.Broadcast(); //更新双方库存
			InventoryUpdate.Broadcast(); //更新双方库存
			return true;
		}
		else
		{
			return true;  //跳过当前执行
		}
	}
	return false;
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



void UISItemsContainer::ToPickUpItemsInBackPack(const FItemInformation Information)
{
	/*物品拾取的一些情况
* 优先搜索背包里含有的相同物品
* 1.如果物品可堆叠，则该背包的位置的物品数量+1
* 2.如果物品不可堆叠，则寻找一个空位放置
* 如果没有相同的物品，则搜索背包内的空位进行防止
* 以上情况都不满足，只能是背包爆满
*/
	if(InventoryContainer.IsEmpty()) {return;} //库存器错误
	TArray<FItemInformation> AddArray;
	if(Information.CanStack == true)
	{
		for(int32 index = 0; index < InventoryContainer.Num(); index++)
		{
			if(InventoryContainer[index].ItemID==Information.ItemID)
			{
				InventoryContainer[index].ItemQuantity+=Information.ItemQuantity;  //物品数量相加
				AddArray.Add(Information);
				SendAddDataToServer(AddArray);  //提交给服务器
				return;
			}
		}
		for(int32 index = 0; index < InventoryContainer.Num(); index++)
		{
			if(InventoryContainer[index].ItemID==-1)
			{
				InventoryContainer[index] = Information;
				AddArray.Add(Information);
				SendAddDataToServer(AddArray);  //提交给服务器
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
				AddArray.Add(Information);
				SendAddDataToServer(AddArray);  //提交给服务器
				return;
			}
		}
	}
}


void UISItemsContainer::SendAddDataToServer_Implementation(const TArray<FItemInformation>& InItems)
{
	UISInventorySystem*OwnInventory = *Inventories.Find(InventoryConfig.ContainerName);
	OwnInventory->AddItems(InItems);  //传输外层的数据到内层，告诉内层添加了数据
}

void UISItemsContainer::SendRemoveDataToServer_Implementation(const TArray<FItemInformation>& InItems)
{
	UISInventorySystem*OwnInventory = *Inventories.Find(InventoryConfig.ContainerName);
	OwnInventory->RemoveItems(InItems);  //传输外层的数据到内层，告诉内层删除了数据
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
			InventoryUpdate.Broadcast();
			return;
		}
		InventoryUpdate.Broadcast();
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

void UISItemsContainer::CallContainerUpdateOnClient_Implementation(const UISItemsContainer* TargetItemsContainer)
{
	TargetItemsContainer->InventoryUpdate.Broadcast();
}

void UISItemsContainer::AddRelevantPlayer(APlayerController* TargetOwner)
{
	if(!RelevantPlayers.Contains(TargetOwner))
	{
		RelevantPlayers.Add(TargetOwner);
	}
}

void UISItemsContainer::RemoveRelevantPlayer(APlayerController* TargetOwner)
{
	if(RelevantPlayers.Contains(TargetOwner))
	{
		RelevantPlayers.Remove(TargetOwner);
	}
}

// Called when the game starts
void UISItemsContainer::BeginPlay()
{
	Super::BeginPlay();
	ItemDiscard.AddDynamic(this,&UISItemsContainer::DiscardItem);
	InitializeContainer();
}
void UISItemsContainer::InitializeContainer()
{
	if(GetOwner()->HasAuthority())
	{
		AISCharacter* SourceCharacter = Cast<AISCharacter>(GetOwner());
		AISPlayerController* PlayerController = Cast<AISPlayerController>(UGameplayStatics::GetPlayerController(SourceCharacter,0));
		if(!RelevantPlayers.Contains(PlayerController))
		{
			RelevantPlayers.Add(PlayerController);
		}
		
		UISStorageDataConfig* StorageDataConfig = UISAbilitysystemLibary::GetStorageDataConfig(this);
		for(auto&StorageRef : StorageDataConfig->GetInventoryConfig(ContainerType)->Inventory)
		{
			if(ContainerName == StorageRef.ContainerName)
			{
				InventoryConfig = StorageRef;
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnComponentCreated"));
		if(!Inventories.Contains(InventoryConfig.ContainerName))  //查询背包组件管理的背包
		{
			auto NewInventory = UISInventorySystem::CreateInventory(this,InventoryConfig);
			Inventories.Add(InventoryConfig.ContainerName,NewInventory);
			if(InventoryConfig.bReplicates)
			{
				RepInventories.Add(NewInventory);  //需要进行复制的库存需要单独添加到用于处理复制的数组里
			}
		}
	}
	InitializeContainerSpace(InventoryConfig.InventorySpace);  //外层创建格子
}


void UISItemsContainer::OnComponentCreated()
{
	Super::OnComponentCreated();
}

void UISItemsContainer::InitializeContainerSpace(const int32 Space)
{
	for(int32 Index = 0;Index<Space;Index++)
	{
		InventoryContainer.Add(ItemInfo);  //添加背包空位栏
		FString ItemNameToPrint = FString::Printf(TEXT("已添加: %s"), *ItemInfo.ItemName.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *ItemNameToPrint);
	}
}

//处理道具同步，执行网络连接同步，将子对象(Replicated)数据发送到网络（运行于服务器）
bool UISItemsContainer::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool Result = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	if(CheckConnection(Channel))
	{
		for(auto& Inventory: RepInventories)  //输送用于网络复制的数组
		{
			Result |= Inventory->ReplicateSubobjects(Channel, Bunch, RepFlags);  //传输需要传到网络的背包
			Result |= Channel->ReplicateSubobject(Inventory,*Bunch,*RepFlags);  //传输所有复制的成员
		}
	}
	return Result;
}

//不会是所有玩家都需要同步，例如当宝箱打开时，打开的玩家需要同步，而未打开的玩家不需要
bool UISItemsContainer::CheckConnection(UActorChannel* Channel)
{
	if(Channel&&Channel->Connection && Channel->Connection->OwningActor)
	{
		return RelevantPlayers.Contains(Channel->Connection->OwningActor);
	}
	return false;
}


// Called every frame
void UISItemsContainer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UISItemsContainer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UISItemsContainer,InventoryConfig);
}

EContainerType UISItemsContainer::GetTargetContainerType()
{
	return InventoryConfig.ContainerType;
}

FName UISItemsContainer::GetContainerName()
{
	return InventoryConfig.ContainerName;
}

void UISItemsContainer::PickUpItemForActor(APawn* TargetPawn, AActor* TargetActor)
{
	AISItemBase*SourceItem = Cast<AISItemBase>(TargetActor);
	FName TargetItemID = SourceItem->ItemID;
	PickUpItemForID(TargetPawn,TargetItemID,1);
}