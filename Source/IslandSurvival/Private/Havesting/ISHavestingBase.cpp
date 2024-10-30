// Fill out your copyright notice in the Description page of Project Settings.


#include "Havesting/ISHavestingBase.h"

// Sets default values
AISHavestingBase::AISHavestingBase()
{
	HarvestStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("HarvestStaticMesh");
	PrimaryActorTick.bCanEverTick = false;  //该类不需要Tick

}

// Called when the game starts or when spawned
void AISHavestingBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AISHavestingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

