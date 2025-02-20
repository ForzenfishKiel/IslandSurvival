// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ISEquipable.h"

#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Character/ISCharacter.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISGameplayMode.h"
#include "Net/UnrealNetwork.h"
#include "Game/ISGameplayTagsManager.h"
#include "Kismet/GameplayStatics.h"


USceneComponent* AISEquipable::GetAttachTarget(APawn* TargetPawn) const
{
	if(!TargetPawn) return nullptr;
	USceneComponent*AttachTarget = TargetPawn->GetRootComponent();
	if(AISCharacter*TargetCharacter = Cast<AISCharacter>(TargetPawn))
	{
		AttachTarget = TargetCharacter->ArmMesh;
		
	}
	return AttachTarget;
}

USceneComponent* AISEquipable::GetAttachThirdPersonParent(APawn* TargetPawn) const
{
	USceneComponent*AttachTarget = TargetPawn->GetRootComponent();
	if(AISCharacter*TargetCharacter = Cast<AISCharacter>(TargetPawn))
	{
		AttachTarget = TargetCharacter->GetMesh();
	}
	return AttachTarget;
}

void AISEquipable::SetEquipableCollision()
{
	PickUpCheckSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemsStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemsStaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Ignore);
}

void AISEquipable::OnRep_AmmoChanged()
{
	AISCharacter* OwnerCharacter = Cast<AISCharacter>(GetOwner());
	check(OwnerCharacter);

	UISHotBarInventory* CharacterHotBar = OwnerCharacter->GetComponentByClass<UISHotBarInventory>();
	CharacterHotBar->UpdateAmmos(Ammos,MaxAmmos);
}

int32 AISEquipable::CheckMagazine(const int32 TargetID)
{
	if(TargetID == -1) return -1;
	AISCharacter* OwnerCharacter = Cast<AISCharacter>(GetOwner());
	check(OwnerCharacter);
	UISCharacterInventory* CharacterInventory = OwnerCharacter->GetComponentByClass<UISCharacterInventory>();
	UISHotBarInventory* CharacterHotBar = OwnerCharacter->GetComponentByClass<UISHotBarInventory>();
	if(!CharacterInventory && !CharacterHotBar) return -1;
	for(int32 i = 0; i < CharacterInventory->InventoryContainer.Num(); i++)
	{
		if(CharacterInventory->InventoryContainer[i].ItemID == TargetID)
		{
			return i;
		}
	}
	for(int32 i = 0; i < CharacterHotBar->InventoryContainer.Num(); i++)
	{
		if(CharacterHotBar->InventoryContainer[i].ItemID == TargetID)
		{
			return i;
		}
	}
	return -1;
}

void AISEquipable::UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC)
{
	Super::UseItem(TargetCharacter, TargetASC);
	const FGameplayTagsManager TagsManager = FGameplayTagsManager::Get();
	ApplyEffectToTarget(TargetASC,EquipableDefaultAttribute);
	AddTargetAbility(TargetASC,EquipableActivateAbilities);//应用主动技能
	AddTargetPassiveAbility(TargetASC,EquibablePassiveAbilities);  //应用被动技能
}

//初始化加载武器配置
void AISEquipable::InitializeEquipableConfig(const FItemInformation&TargetInfo)
{
	ItemID = FName(FString::Printf(TEXT("%d"),TargetInfo.ItemID));
	ItemType = TargetInfo.ItemType;
	ItemRarity = TargetInfo.TargetItemRarity;
}

void AISEquipable::UnUseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC)
{
	Super::UnUseItem(TargetCharacter, TargetASC);
	RemoveTargetAbility(TargetASC,EquipableActivateAbilities);
	RemoveTargetAbility(TargetASC,EquibablePassiveAbilities);
	RemoveTargetEffect(TargetASC,EquipableDefaultAttribute);
}

//初始化角色属性
void AISEquipable::ApplyEffectToTarget(UAbilitySystemComponent* InASC, TSubclassOf<UGameplayEffect> EffectClass)
{
	Super::ApplyEffectToTarget(InASC, EffectClass);
	check(EffectClass);
	FGameplayEffectContextHandle GameplayEffectContext = InASC->MakeEffectContext();
	GameplayEffectContext.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = InASC->MakeOutgoingSpec(EffectClass,1.f,GameplayEffectContext); //获取规范
	const FActiveGameplayEffectHandle ActivateHandle = InASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
}

void AISEquipable::AddTargetAbility(UAbilitySystemComponent* TargetASC,
	TArray<TSubclassOf<UGameplayAbility>>& TargetArray)
{
	Super::AddTargetAbility(TargetASC, TargetArray);
	UISAbilitySystemComponent*LocalASC = Cast<UISAbilitySystemComponent>(TargetASC);
	if(!LocalASC&&TargetArray.IsEmpty())return;
	LocalASC->AddCharacterAbility(TargetArray);
}

void AISEquipable::AddTargetPassiveAbility(UAbilitySystemComponent* TargetASC,
	TArray<TSubclassOf<UGameplayAbility>>& TargetArray)
{
	Super::AddTargetPassiveAbility(TargetASC, TargetArray);
	UISAbilitySystemComponent*LocalASC = Cast<UISAbilitySystemComponent>(TargetASC);
	if(!LocalASC&&TargetArray.IsEmpty())return;
	LocalASC->AddCharacterPassiveAbility(TargetArray);
}

void AISEquipable::RemoveTargetAbility(UAbilitySystemComponent* TargetASC,
                                       TArray<TSubclassOf<UGameplayAbility>>& TargetArray)
{
	Super::RemoveTargetAbility(TargetASC, TargetArray);
	UISAbilitySystemComponent*LocalASC = Cast<UISAbilitySystemComponent>(TargetASC);
	if(!LocalASC&&TargetArray.IsEmpty())return;
	LocalASC->RemoveCharacterAbility(TargetArray);
}

void AISEquipable::RemoveTargetEffect(UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> EffectClass)
{
	Super::RemoveTargetEffect(TargetASC, EffectClass);
	UISAbilitySystemComponent*LocalASC = Cast<UISAbilitySystemComponent>(TargetASC);
	if(!LocalASC&&!IsValid(EffectClass))return;
	LocalASC->RemoveCharacterAttribute(EffectClass);
}

ECharacterEquipState AISEquipable::GetTargetEquipState()
{
	return EquipState;
}

EItemRarity AISEquipable::GetItemRarity_Implementation()
{
	return ItemRarity;
}

void AISEquipable::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AISEquipable,Ammos);
}

AISEquipable* AISEquipable::GetEquipable_Implementation()
{
	return this;
}

void AISEquipable::OnEquipableWasInteract_Implementation(AActor* InteractingActor)
{
	IISEquipableInterface::OnEquipableWasInteract_Implementation(InteractingActor);
}

//检查采集的物品是否在数据表中 
TSubclassOf<AISHarvestingBase> AISEquipable::CheckHarvestDataAsset(const FString& InName)
{
	AISGameplayMode* SourceGamemomde = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(GetOwner()));
	if(!SourceGamemomde)return nullptr;

	UISHarvestDataAsset* HarvestDataAsset = UISAbilitysystemLibary::GetHarvestDataAsset(GetOwner());
	if(!HarvestDataAsset)return nullptr;

	return HarvestDataAsset->GetHarvest(InName); //返回对应的类模板
}

AISHarvestingBase* AISEquipable::ReplaceToActor(const TSubclassOf<AISHarvestingBase> InClassTemp
	,UInstancedStaticMeshComponent* TargetComponent, const int32 TargetItemNum)
{
	FTransform OutTransform;
	if(!TargetComponent->GetInstanceTransform(TargetItemNum,OutTransform,true)) return nullptr;
	MulticastToRemoveStaticMesh(TargetComponent,TargetItemNum); //删除原来的模型
	
	AISHarvestingBase* HarvestingBaseRef = GetWorld()->SpawnActor<AISHarvestingBase>(InClassTemp,OutTransform); //在原位置生成Actor
	return HarvestingBaseRef;
}

void AISEquipable::MulticastToRemoveStaticMesh_Implementation(UInstancedStaticMeshComponent* TargetComponent,
	const int32 TargetItemNum)
{
	if(!IsValid(TargetComponent) && TargetItemNum <= 0) return;
	TargetComponent->RemoveInstance(TargetItemNum);
}
