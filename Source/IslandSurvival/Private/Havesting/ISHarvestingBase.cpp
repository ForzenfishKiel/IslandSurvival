// Fill out your copyright notice in the Description page of Project Settings.


#include "Havesting/ISHarvestingBase.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Net/UnrealNetwork.h"
#include "Character/ISCharacter.h"
#include "Game/ISAttributeSet.h"
#include "Game/ISPlayerState.h"

// Sets default values
AISHarvestingBase::AISHarvestingBase()
{
	HarvestStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("HarvestStaticMesh");
	HarvestStaticMesh->SetCollisionObjectType(ECC_GameTraceChannel2);
	HarvestStaticMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	HarvestStaticMesh->SetIsReplicated(true);
	PrimaryActorTick.bCanEverTick = false;  //该类不需要Tick
	bReplicates = true;
	CollectibleHP = CollectibleMaxHP;
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
	DOREPLIFETIME(AISHarvestingBase, CollectibleName);
	DOREPLIFETIME(AISHarvestingBase,CollectibleHP);
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
	AISCharacter* SourceCharacter = Cast<AISCharacter>(TargetActor);
	if(!SourceCharacter) return;  //如果为目标角色
	UISItemsContainer* ItemContainer = SourceCharacter->GetComponentByClass<UISItemsContainer>();
	
	UISDropCollectibleDataAsset* CollectibleDataAsset = UISAbilitysystemLibary::GetDropCollectibleDataAsset(this);
	const FDropInformation DropInfo = CollectibleDataAsset->GetDropConfig(CollectibleClass);
	
	//计算掉落

	for(auto& TargetRef:DropInfo.Drops)
	{
		if(TargetRef.TargetName==CollectibleName)
		{
			for(auto& TargetDropRef:TargetRef.Drops)
			{
				if(TargetDropRef.TargetID!=FName("None"))  //对应ID不为-1，防止出错
				{
					/*则开始计算掉落概率和掉落数量*/

					const bool bCanDrop= FMath::RandRange(0,100) < TargetDropRef.DropRate.GetValueAtLevel(1.f);  //获取掉落概率
					if(bCanDrop)
					{
						CallMulticastDelegates();  //广播播放效果
						ItemContainer->PickUpItemForID
						(SourceCharacter,TargetDropRef.TargetID,Execute_GetNumsFromMultiplier(this,TargetTool,TargetDropRef.DropNums.GetValueAtLevel(1.f)));
					}
				}
			}
		}
	}
	
	Execute_ApplyDamageToTarget(this,TargetActor); //计算伤害
}

//服务器运行
void AISHarvestingBase::ApplyDamageToTarget_Implementation(AActor* TargetActor)
{
	IISCollectibleInterface::ApplyDamageToTarget_Implementation(TargetActor);
	AISCharacter*SourceCharacter = Cast<AISCharacter>(TargetActor);
	AISPlayerState*SourcePlayerState = SourceCharacter->GetPlayerState<AISPlayerState>();
	if(!SourcePlayerState&&!SourceCharacter) return;
	UAbilitySystemComponent*SourceASC = SourcePlayerState->GetAbilitySystemComponent();
	UISAttributeSet*SourceAS = Cast<UISAttributeSet>( SourcePlayerState->GetAttributeSet());  //获取角色的属性
	const float ToolDamage = SourceAS->GetGatheringDamage();  //角色使用工具采集，计算工具的伤害
	if(ToolDamage>0)  //如果武器伤害大于0
	{
		CollectibleHP = FMath::Clamp(CollectibleHP-ToolDamage,0.f,CollectibleMaxHP) ;  //血量相减
	}
	if(CollectibleHP==0)  //采集物血量为0的时候停止采集，并执行一些功能
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("采集完毕"));
		if(HarvestStaticMesh)
		{
			HarvestStaticMesh->SetCollisionObjectType(ECC_WorldDynamic);
		}
		EnableHarvestionBreak(TargetActor);
	}
}
//计算实际采集的数量（提供武器，人物等级等）
int32 AISHarvestingBase::GetNumsFromMultiplier_Implementation(AActor* TargetTool, int32 TargetNums)
{
	
	UISEquipableDataAsset*TargetDataAsset = UISAbilitysystemLibary::GetEquipableDataAsset(this);
	if(!TargetDataAsset) return 0;
	//角色不使用工具采集
	if(TargetTool->Implements<UISCombatInterface>())
	{
		FEquipItemRarityContainer EquipItemRarity = TargetDataAsset->GetTargetItemRarityContainer(FName("0"));
		const float BoostMultiplier = EquipItemRarity.RarityContainer[0].BootMultiplier.GetValue();
		const int32 Result = FMath::RoundToFloat(TargetNums * BoostMultiplier);
		return Result;
	}
	
	AISCharacter*SourceCharacter = Cast<AISCharacter>(TargetTool->GetOwner());
	if(!SourceCharacter) return 0;
	AISPlayerState*SourcePlayerState = SourceCharacter->GetPlayerState<AISPlayerState>();
	UISAttributeSet*SourceAS = Cast<UISAttributeSet>( SourcePlayerState->GetAttributeSet());  //获取角色的属性


	//角色使用工具采集
	FEquipItemRarityContainer EquipItemRarity = TargetDataAsset->GetTargetItemRarityContainer(IISItemInterface::Execute_GetName(TargetTool));
	for(auto&EquipItemRarityContainer:EquipItemRarity.RarityContainer)
	{
		if(EquipItemRarityContainer.ItemRarity==IISEquipableInterface::Execute_GetItemRarity(TargetTool))
		{
			const float ToolDamage = SourceAS->GetGatheringDamage();  //角色使用工具采集，计算工具的伤害
			const float BoostMultiplier = EquipItemRarityContainer.BootMultiplier.GetValueAtLevel(1.f);
			const int32 Result = FMath::RoundToFloat(TargetNums * BoostMultiplier + TargetNums * (ToolDamage / 10.f));
			return Result;
		}
	}
	return 0;
}
