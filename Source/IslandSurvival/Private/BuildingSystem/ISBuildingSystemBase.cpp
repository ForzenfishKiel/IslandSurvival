// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/ISBuildingSystemBase.h"

// Sets default values
AISBuildingSystemBase::AISBuildingSystemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PickUpCheckSphere->UnregisterComponent();
	PickUpCheckSphere->DestroyComponent();
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);
	ItemsStaticMesh->SetupAttachment(RootSceneComponent);
	RootSceneComponent->SetMobility(EComponentMobility::Movable);
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	BoxCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);//所有通道全忽略
	BoxCollisionComponent->SetupAttachment(RootSceneComponent);
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

//获取建筑的吸附判定框（数组）
TArray<UBoxComponent*> AISBuildingSystemBase::GetBuildingBoxComponent_Implementation() const
{
	TArray<UBoxComponent*> ISBoxComponent;
	for(auto&CompRef:GetComponents())
	{
		UStaticMeshComponent*BuildMeshRef = Cast<UStaticMeshComponent>(CompRef);
		if(BuildMeshRef)
		{
			for(auto&StaticMeshChildRef:BuildMeshRef->GetAttachChildren())
			{
				UBoxComponent* BoxCompRef = Cast<UBoxComponent>(StaticMeshChildRef);
				if(BoxCompRef)
				{
					ISBoxComponent.Add(BoxCompRef);
				}
			}
		}
	}
	return ISBoxComponent;
}

void AISBuildingSystemBase::OnBuildingWasInteract_Implementation(const AActor* InteractingActor,
	const UActorComponent* InteractingComponent)
{
	IISBuildInterface::OnBuildingWasInteract_Implementation(InteractingActor, InteractingComponent);
}

