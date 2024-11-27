// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/ISBuildingSystemBase.h"

// Sets default values
AISBuildingSystemBase::AISBuildingSystemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AISBuildingSystemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AISBuildingSystemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

