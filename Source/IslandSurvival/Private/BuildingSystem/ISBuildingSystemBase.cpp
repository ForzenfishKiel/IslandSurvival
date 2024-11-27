// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/ISBuildingSystemBase.h"

// Sets default values
AISBuildingSystemBase::AISBuildingSystemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PickUpCheckSphere->UnregisterComponent();
	PickUpCheckSphere->DestroyComponent();
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	RootSceneComponent->SetupAttachment(GetRootComponent());
	ItemsStaticMesh->SetupAttachment(RootSceneComponent);
	RootSceneComponent->SetMobility(EComponentMobility::Movable);
	bReplicates = true;
}

// Called when the game starts or when spawned
void AISBuildingSystemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

AISBuildingSystemBase* AISBuildingSystemBase::GetBuildingSystemBase_Implementation()
{
	return this;
}

