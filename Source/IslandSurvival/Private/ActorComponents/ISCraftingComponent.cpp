// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISCraftingComponent.h"

#include "Character/ISCharacter.h"
#include "Game/ISGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UISCraftingComponent::UISCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

int32 UISCraftingComponent::FindCheckCharacterBackPack(const UDataTable* TargetDT, const int32 TargetID,
	const int32 RequireID)
{
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetOwner());
	UISHotBarInventory*CharacterHotBar = SourceCharacter->GetComponentByClass<UISHotBarInventory>();
	UISCharacterInventory*CharacterInventory = SourceCharacter->GetComponentByClass<UISCharacterInventory>();
	
	//本地运行
	UISGameInstance*ISGameplayInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));  //获取本地游戏实例
	if(!ISGameplayInstance) return 0;
	UISCraftingContainer*CraftingContainer = ISGameplayInstance->ItemContainer;
	if(!CraftingContainer) return 0;
	int32 Result = 0;
	FName Trans = FName(FString::Printf(TEXT("%d"),TargetID));
	
	if(FItemRecipe*UserInfo = TargetDT->FindRow<FItemRecipe>(Trans,TEXT("name")))
	{
		for(auto&DataTableRef:UserInfo->ItemRequired)
		{
			if(DataTableRef.ItemID!=RequireID) continue;
			for(int32 Index = 0;Index<CharacterHotBar->InventoryContainer.Num();Index++)
			{
				if(CharacterHotBar->InventoryContainer[Index].ItemID==RequireID)
				{
					const int32 Value = CharacterHotBar->InventoryContainer[Index].ItemQuantity;
					Result += Value;
				}
			}
			for(int32 Index = 0;Index<CharacterInventory->InventoryContainer.Num();Index++)
			{
				if(CharacterInventory->InventoryContainer[Index].ItemID==RequireID)
				{
					const int32 Value = CharacterInventory->InventoryContainer[Index].ItemQuantity;
					Result += Value;
				}
			}
			return Result;
		}
	}
	return 0;
}

bool UISCraftingComponent::IsCanBeCrafting(const UDataTable* TargetDT, const int32 TargetID, const int32 RequireID)
{
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetOwner());
	UISHotBarInventory*CharacterHotBar = SourceCharacter->GetComponentByClass<UISHotBarInventory>();
	UISCharacterInventory*CharacterInventory = SourceCharacter->GetComponentByClass<UISCharacterInventory>();
	
	//本地运行
	UISGameInstance*ISGameplayInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));  //获取本地游戏实例
	if(!ISGameplayInstance) return false;
	UISCraftingContainer*CraftingContainer = ISGameplayInstance->ItemContainer;
	if(!CraftingContainer) return false;
	int32 Result = 0;
	FName Trans = FName(FString::Printf(TEXT("%d"),TargetID));
	
	if(FItemRecipe*UserInfo = TargetDT->FindRow<FItemRecipe>(Trans,TEXT("name")))
	{
		for(auto&DataTableRef:UserInfo->ItemRequired)
		{
			if(DataTableRef.ItemID!=RequireID) continue;
			for(int32 Index = 0;Index<CharacterHotBar->InventoryContainer.Num();Index++)
			{
				if(CharacterHotBar->InventoryContainer[Index].ItemID==RequireID)
				{
					const int32 Value = CharacterHotBar->InventoryContainer[Index].ItemQuantity;
					Result += Value;
				}
			}
			for(int32 Index = 0;Index<CharacterInventory->InventoryContainer.Num();Index++)
			{
				if(CharacterInventory->InventoryContainer[Index].ItemID==RequireID)
				{
					const int32 Value = CharacterInventory->InventoryContainer[Index].ItemQuantity;
					Result += Value;
				}
			}
			if(Result>=DataTableRef.ItemQuantity)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

void UISCraftingComponent::CraftingAction(const UDataTable*TargetDT,const int32 TargetID)
{
	UISGameInstance*ISGameplayInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));  //获取本地游戏实例
	if(!ISGameplayInstance) return;
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetOwner());
	UISHotBarInventory*CharacterHotBar = SourceCharacter->GetComponentByClass<UISHotBarInventory>();  //获取物品栏
	UISCharacterInventory*CharacterInventory = SourceCharacter->GetComponentByClass<UISCharacterInventory>();  //获取背包
	UISItemsContainer*ItemsContainer = SourceCharacter->GetComponentByClass<UISItemsContainer>();  //获取系统库存组件

	FName Trans = FName(FString::Printf(TEXT("%d"),TargetID));
	int32 Result = 0;
	if(FItemRecipe*UserInfo = TargetDT->FindRow<FItemRecipe>(Trans,TEXT("name")))
	{
		for(auto&DataTableRef:UserInfo->ItemRequired)
		{
			for(int32 Index = 0;Index<CharacterHotBar->InventoryContainer.Num();Index++)
			{
				if(CharacterHotBar->InventoryContainer[Index].ItemID==DataTableRef.ItemID)
				{
					const int32 Value = CharacterHotBar->InventoryContainer[Index].ItemQuantity;
					Result+=Value;
					if(Result==DataTableRef.ItemQuantity){CharacterHotBar->DiscardItem(Index,Result);  Result = 0; break;}
					if(Result>DataTableRef.ItemQuantity){CharacterHotBar->DiscardItem(Index,DataTableRef.ItemQuantity); Result = 0; break;}
					CharacterHotBar->DiscardItem(Index,Value);
				}
			}
			for(int32 Index = 0;Index<CharacterInventory->InventoryContainer.Num();Index++)
			{
				if(CharacterInventory->InventoryContainer[Index].ItemID==DataTableRef.ItemID)
				{
					const int32 Value = CharacterInventory->InventoryContainer[Index].ItemQuantity;
					Result+=Value;
					if(Result==DataTableRef.ItemQuantity){CharacterInventory->DiscardItem(Index,Result);  Result = 0; break;}
					if(Result>DataTableRef.ItemQuantity){CharacterInventory->DiscardItem(Index,DataTableRef.ItemQuantity); Result = 0; break;}
					CharacterInventory->DiscardItem(Index,Result);
				}
			}
		}
		UDataTable*ItemTable = ISGameplayInstance->ItemDataTable;
		if(FItemInformation*Information = ItemTable->FindRow<FItemInformation>(Trans,TEXT("name")))
		{
			ItemsContainer->PickUpItemForID(SourceCharacter,Trans,1);
		}
	}
	return;
}

// Called when the game starts
void UISCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UISCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

