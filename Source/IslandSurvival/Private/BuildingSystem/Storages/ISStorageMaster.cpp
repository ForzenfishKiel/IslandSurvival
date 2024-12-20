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


void AISStorageMaster::BeginPlay()
{

	check(StorageUserWidgetClass);
	StorageWidget = CreateWidget<UISMenuUIBase>(GetWorld(),StorageUserWidgetClass);  //创建UI
	StorageWidget->SetWidgetOwner(this);
}

void AISStorageMaster::OnBuildingWasInteract_Implementation(const AActor* InteractingActor,
	const UActorComponent* InteractingComponent)
{
	APlayerController* SourcePC = Cast<APlayerController>( InteractingActor->GetInstigatorController());
	if(SourcePC)
	{
		StorageUIOpen(SourcePC);  //客户端打开UI
	}
}


void AISStorageMaster::StorageUIOpen(APlayerController* TargetController)
{
	check(StorageWidget);
	AISPlayerController* SourcePC = Cast<AISPlayerController>(TargetController);  //转换成本地PC
	if(!SourcePC) return;
	SourcePC->OnOpenInventoryEvent.AddDynamic(this,&AISStorageMaster::StorageUIClose);
	
	
	StorageWidget->InitializeCraftingData(StorageContainer);
	if(!StorageWidget->IsVisible())
	{
		SourcePC->bIsOpenStorage = true;
		StorageWidget->AddToViewport();
		SourcePC->bShowMouseCursor = true;
		SourcePC->SetInputMode(FInputModeGameAndUI());
		SourcePC->InputSubsystem->RemoveMappingContext(SourcePC->CharacterInputMapping);
		SourcePC->InputSubsystem->AddMappingContext(SourcePC->CharacterInputMenuMapping,0);
	}
}

void AISStorageMaster::StorageUIClose(APlayerController* TargetController)
{
	AISPlayerController* SourcePC = Cast<AISPlayerController>(TargetController);
	if(!SourcePC) return;

	
	if(StorageWidget->IsVisible())
	{
		SourcePC->bIsOpenStorage = false;
		StorageWidget->RemoveFromParent();
		SourcePC->bShowMouseCursor = false;
		SourcePC->SetInputMode(FInputModeGameOnly());
		SourcePC->InputSubsystem->RemoveMappingContext(SourcePC->CharacterInputMenuMapping);
		SourcePC->InputSubsystem->AddMappingContext(SourcePC->CharacterInputMapping,0);
		SourcePC->OnOpenInventoryEvent.RemoveDynamic(this,&AISStorageMaster::StorageUIClose);
	}
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
