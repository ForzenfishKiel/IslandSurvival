// Fill out your copyright notice in the Description page of Project Settings.


#include "Havesting/ISHarvestingBase.h"

// Sets default values
AISHarvestingBase::AISHarvestingBase()
{
	HarvestStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("HarvestStaticMesh");
	HarvestStaticMesh->SetCollisionObjectType(ECC_GameTraceChannel2);
	HarvestStaticMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	PrimaryActorTick.bCanEverTick = false;  //该类不需要Tick
	bReplicates = true;
}
// Called when the game starts or when spawned
void AISHarvestingBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AISHarvestingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
FName AISHarvestingBase::GetTargetName()
{
	return CollectibleName;
}

ECollectibleClass AISHarvestingBase::GetTargetClass_Implementation()
{
	return CollectibleClass;
}
