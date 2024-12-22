// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/ActorComponents/ISStroageContainer.h"

#include "Character/ISCharacter.h"
#include "Data/ISCraftItemInformation.h"


//制造按钮点击的时候会执行这个函数，这个函数的任务是查询玩家以及制造台库存的背包里的所有可用于制造的物品，一旦查询成功则扣除相应的物品
void UISStroageContainer::CraftAction(UISCraftingComponent* TargetCraftingComponent, const UDataTable* TargetDT,
                                      const int32 TargetID)
{
	//检查是否是本地玩家执行
	AActor* Owner = TargetCraftingComponent->GetOwner();
	AISCharacter* SourceCharacter = Cast<AISCharacter>(Owner);
	if(!SourceCharacter) return;
	APlayerController* SourceController = Cast<APlayerController>(SourceCharacter->GetController());
	if(!SourceController && !SourceController->IsLocalPlayerController()) return;
	
	UISHotBarInventory* CharacterHotBar = SourceCharacter->GetComponentByClass<UISHotBarInventory>();  //获取角色物品栏组件
	UISCharacterInventory* CharacterBackPack = SourceCharacter->GetComponentByClass<UISCharacterInventory>();  //获取角色背包组件
	if(!CharacterHotBar && !CharacterBackPack) return;
	//

	FName Trans = FName(FString::Printf(TEXT("%d"),TargetID));  //转换传入的int32类型的ID进行数据表的检索
	int32 Result = 0;  //定义一个临时的结果数值，用于累加在不同位置查询到的物品
	FItemRecipe*UserInfo = TargetDT->FindRow<FItemRecipe>(Trans,TEXT("name"));
	check(UserInfo);

	for(auto&DataTableRef : UserInfo->ItemRequired)
	{
		//优先查询工作台的库存
		for(int32 Index = 0; Index < InventoryContainer.Num(); Index++)
		{
			if(InventoryContainer[Index].ItemID == DataTableRef.ItemID)
			{
				const int32 Value = InventoryContainer[Index].ItemQuantity;
				Result += Value;
				if(Result == DataTableRef.ItemQuantity)
				{
					DiscardItem(Index,Result);
					Result = 0;
					break;  //跳过内层循环
				}
				if(Result > DataTableRef.ItemQuantity)
				{
					DiscardItem(Index,Value);
					Result = 0;
					break;
				}
				
				DiscardItem(Index,Value);
			}
		}
	}
}

void UISStroageContainer::BeginPlay()
{
	
}

