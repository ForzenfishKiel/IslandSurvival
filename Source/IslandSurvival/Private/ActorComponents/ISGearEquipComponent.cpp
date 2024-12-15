// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISGearEquipComponent.h"
#include "Net/UnrealNetwork.h"

void UISGearEquipComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UISGearEquipComponent::InitializeEquipContainer()
{
	GearEquipContainer.Add(EArmorType::Helmet,ItemInfo);
	GearEquipContainer.Add(EArmorType::Chest,ItemInfo);
	GearEquipContainer.Add(EArmorType::Pants,ItemInfo);
	GearEquipContainer.Add(EArmorType::Boots,ItemInfo);
}
