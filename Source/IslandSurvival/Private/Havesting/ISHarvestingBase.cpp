// Fill out your copyright notice in the Description page of Project Settings.


#include "Havesting/ISHarvestingBase.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Net/UnrealNetwork.h"
#include "Character/ISCharacter.h"

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

void AISHarvestingBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AISHarvestingBase, CollectibleClass);
	DOREPLIFETIME(AISHarvestingBase, CollectibleHP);
	DOREPLIFETIME(AISHarvestingBase, CollectibleMaxHP);
	DOREPLIFETIME(AISHarvestingBase, CollectibleName);
}

FName AISHarvestingBase::GetTargetName()
{
	return CollectibleName;
}

ECollectibleClass AISHarvestingBase::GetTargetClass_Implementation()
{
	return CollectibleClass;
}

void AISHarvestingBase::CollectionExecution_Implementation(AActor* TargetActor, AActor* TargetTool)
{
	IISCollectibleInterface::CollectionExecution_Implementation(TargetActor, TargetTool);
	AISCharacter*SourceCharacter = Cast<AISCharacter>(TargetActor);
	if(!SourceCharacter) return;  //如果为目标角色
	UISItemsContainer*ItemContainer = SourceCharacter->GetComponentByClass<UISItemsContainer>();
	UISCollectibleDataAsset*CollectibleDataAsset = UISAbilitysystemLibary::GetCollectibleDataAsset(this);
	const FDropInformation DropInfo = CollectibleDataAsset->GetDropConfig(CollectibleClass);
	for(auto&TargetRef:DropInfo.Drops)
	{
		if(TargetRef.TargetName==CollectibleName)
		{
			for(auto&TargetDropRef:TargetRef.Drops)
			{
				if(TargetDropRef.TargetID!=FName("None"))  //对应ID不为-1，防止出错
				{
					/*则开始计算掉落概率和掉落数量*/
					const bool bCanDrop= FMath::RandRange(0,100) < TargetDropRef.DropRate.GetValueAtLevel(1.f);  //获取掉落概率
					if(bCanDrop)
					{
						ItemContainer->PickUpItemForID(SourceCharacter,TargetDropRef.TargetID,TargetDropRef.DropNums.GetValueAtLevel(1.f));
					}
				}
			}
		}
	}
}