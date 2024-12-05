// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Storages/ISStorageMaster.h"

AISStorageMaster::AISStorageMaster()
{
	StorageContainer = CreateDefaultSubobject<UISStroageContainer>(TEXT("StorageContainer"));
	StorageContainer->SetIsReplicated(true);
}
