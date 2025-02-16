// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Storages/ISStorageMaster.h"

#include "Game/ISGameplayMode.h"
#include "Game/ISPlayerController.h"
#include "Kismet/GameplayStatics.h"

AISStorageMaster::AISStorageMaster()
{
	StorageContainer = CreateDefaultSubobject<UISStroageContainer>(TEXT("StorageContainer"));
	StorageContainer->SetIsReplicated(true);
}

void AISStorageMaster::InitItemConfig()
{
	Super::InitItemConfig();
	check(StorageUserWidgetClass);
	StorageWidget = CreateWidget<UISMenuUIBase>(GetWorld(),StorageUserWidgetClass);  //创建UI
	StorageWidget->SetWidgetOwner(this);
}

void AISStorageMaster::OnBuildingWasInteract_Implementation(const AActor* InteractingActor,
                                                            const UActorComponent* InteractingComponent)
{
	
}

void AISStorageMaster::InteractOnServer(AController* InPlayerController)
{
	if(StorageContainer->RepInventoryPlayer.Contains(InPlayerController))
	{
		StorageContainer->RepInventoryPlayer.Remove(InPlayerController);
	}
	else
	{
		StorageContainer->RepInventoryPlayer.Add(InPlayerController);
	}
}

UISMenuUIBase* AISStorageMaster::GetMenuUI_Implementation() const
{
	if(StorageWidget)
	{
		return StorageWidget;
	}
	return nullptr;
}

UISItemsContainer* AISStorageMaster::GetItemsContainer_Implementation() const
{
	return StorageContainer;
}

void AISStorageMaster::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	if(Ar.ArIsSaveGame)
	{
		if(StorageContainer)
		{
			Ar << StorageContainer->InventoryContainer;  //反序列化库存组件数组
		}
	}
}

