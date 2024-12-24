// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISGearEquipComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void UISGearEquipComponent::BeginPlay()
{
	Super::BeginPlay();
}

//初始化组件，且不重写父类
void UISGearEquipComponent::InitializeComponent()
{
	FItemInformation NewItemInfo;

	NewItemInfo.ArmorType = EArmorType::Helmet;
	InventoryContainer.Emplace(NewItemInfo);
	NewItemInfo.ArmorType = EArmorType::Chest;
	InventoryContainer.Emplace(NewItemInfo);
	NewItemInfo.ArmorType = EArmorType::Pants;
	InventoryContainer.Emplace(NewItemInfo);
	NewItemInfo.ArmorType = EArmorType::Boots;
	InventoryContainer.Emplace(NewItemInfo);
}