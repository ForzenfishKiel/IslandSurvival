// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ISMenuUIBase.h"

#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Game/ISGameInstance.h"
#include "Game/ISGameplayMode.h"
#include "Kismet/GameplayStatics.h"

void UISMenuUIBase::InitializeCraftingData_Implementation(UObject* Target)
{
	UISGameInstance* ISGameplayInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(!ISGameplayInstance) return;
	
	UISCraftingContainer* CraftingContainer = ISGameplayInstance->ItemContainer;
	if(!CraftingContainer) return;
	
	if(Target->Implements<UISContainerInterface>())
	{
		IISContainerInterface* Container = Cast<IISContainerInterface>(Target);   //目标是否实现了接口
		for(auto& ArrayRef: CraftingContainer->GetCraftingInfoMap(Container->GetTargetContainerType()).CraftingInfos)
		{
			if(ArrayRef.TargetName==Container->GetContainerName())
			{
				CraftingInfo.ItemCraftingInfoDataTable = ArrayRef.ItemCraftingInfoDataTable;
				CraftingInfo.TargetName = ArrayRef.TargetName;
				return;
			}
		}
	}
}

void UISMenuUIBase::SafeToClearData_Implementation()
{
	CraftingInfo.TargetName = FName();
	CraftingInfo.ItemCraftingInfoDataTable = nullptr;
}

void UISMenuUIBase::SetWidgetController(UObject* TargetWidgetController)
{
	WidgetController = TargetWidgetController;
	WidgetControllerWasSet();
}

void UISMenuUIBase::SetWidgetOwner(UObject* TargetWidgetOwner)
{
	WidgetOwner = TargetWidgetOwner;
	WidgetOwnerWasSet();
}

void UISMenuUIBase::ImportGameItemTableData()
{
	UISGameInstance* ISGameInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); //获取游戏实例
	UDataTable* PlayerItemDataTable = ISGameInstance->ItemDataTable;
	CraftingInfo.ItemCraftingInfoDataTable = PlayerItemDataTable;
	ItemTableDataWasSet();
}
