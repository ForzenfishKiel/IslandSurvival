// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ISItemBase.h"
#include "Character/ISCharacter.h"

// Sets default values
AISItemBase::AISItemBase()
{
	PickUpCheckSphere = CreateDefaultSubobject<USphereComponent>("PickUpCheckSphere");
	PickUpCheckSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Block);  //设置游戏拾取检测通道默认为是
	SetRootComponent(PickUpCheckSphere);
	PrimaryActorTick.bCanEverTick = true;
	ItemsStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemsStaticMesh");
	ItemsStaticMesh->SetupAttachment(PickUpCheckSphere);
	ItemsStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bReplicates = true;  //设置为可复制
}

// Called when the game starts or when spawned
void AISItemBase::BeginPlay()
{
	Super::BeginPlay();
	InitItemConfig();
}

// Called every frame
void AISItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AISItemBase::UnUseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC)
{
	
}

FName AISItemBase::GetName_Implementation()
{
	return ItemID;
}

void AISItemBase::InitItemConfig()
{
	
}

void AISItemBase::UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC)
{
	
}
