// Fill out your copyright notice in the Description page of Project Settings.


#include "Havesting/ISLargetItemMaster.h"

AISLargetItemMaster::AISLargetItemMaster()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);
	HarvestStaticMesh->SetupAttachment(RootSceneComponent);
}
