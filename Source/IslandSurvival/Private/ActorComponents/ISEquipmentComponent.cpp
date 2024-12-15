// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISEquipmentComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UISEquipmentComponent::UISEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true); //默认打开复制
}

void UISEquipmentComponent::InitializeEquipmentComponent(UAbilitySystemComponent*TargetASC)
{
	SourceASC = TargetASC;
	OnEquipGear.AddDynamic(this,&UISEquipmentComponent::EquipGear);
	OnUnEquipGear.AddDynamic(this,&UISEquipmentComponent::UnEquipGear);
}

void UISEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UISEquipmentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UISEquipmentComponent, Equipable);
	DOREPLIFETIME(UISEquipmentComponent,CharacterEquipState);
	DOREPLIFETIME(UISEquipmentComponent,ISHelmet);
	DOREPLIFETIME(UISEquipmentComponent,ISChest);
	DOREPLIFETIME(UISEquipmentComponent,ISPants);
	DOREPLIFETIME(UISEquipmentComponent,ISBoots);
}

//在服务器完成武器的初始化
void UISEquipmentComponent::Equip_Implementation(const FItemInformation TargetInformation)
{
	//确定储存的武器数据不为空
	Equipable = GetWorld()->SpawnActorDeferred<AISEquipable>(TargetInformation.ItemClassRef,FTransform::Identity,GetOwner());
	UGameplayStatics::FinishSpawningActor(Equipable,FTransform::Identity);
	Equipable->InitializeEquipableConfig(TargetInformation);  //初始化其配置
	CharacterEquipState = Equipable->EquipState;
	Equipable->UseItem(GetOwner(),SourceASC);
	USceneComponent*AttachThirdPerson = Equipable->GetAttachThirdPersonParent(Cast<APawn>(Equipable->GetOwner()));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("服务器触发: %s"), *Equipable->GetOwner()->GetName()));
	SpawnEquip_Implementation(AttachThirdPerson);
}
// NetMulticast
void UISEquipmentComponent::SpawnEquip_Implementation(USceneComponent* AttachEquip)
{
	Equipable->SetEquipableCollision();
	Equipable->ItemsStaticMesh->bOwnerNoSee = true;
	Equipable->SetActorRelativeTransform(FTransform::Identity);
	Equipable->AttachToComponent(AttachEquip,FAttachmentTransformRules::KeepRelativeTransform,FName("WEAPON_R"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("多播触发！！"));
}

//客户端调用武器装备
void UISEquipmentComponent::SpawnEquipOnClient_Implementation(const FItemInformation TargetInformation)
{
	EquipableClient = GetWorld()->SpawnActorDeferred<AISEquipable>(TargetInformation.ItemClassRef,FTransform::Identity,GetOwner());
	UGameplayStatics::FinishSpawningActor(EquipableClient,FTransform::Identity);
	EquipableClient->SetEquipableCollision();
	EquipableClient->ItemsStaticMesh->bOnlyOwnerSee = true;
	USceneComponent*AttachThirdPerson = EquipableClient->GetAttachTarget(Cast<APawn>(EquipableClient->GetOwner()));
	EquipableClient->SetActorRelativeTransform(FTransform::Identity);
	EquipableClient->AttachToComponent(AttachThirdPerson,FAttachmentTransformRules::KeepRelativeTransform,FName("WEAPON_R"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("客户端触发！！！"));
}

void UISEquipmentComponent::UnEquip_Implementation()
{
	CharacterEquipState = ECharacterEquipState::None;
	Equipable->UnUseItem(GetOwner(),SourceASC);
	Equipable->Destroy();
	Equipable = nullptr;
}
void UISEquipmentComponent::UnEquipOnClient_Implementation()
{
	EquipableClient->Destroy();
	EquipableClient = nullptr;
}
//使用物品
void UISEquipmentComponent::UseConsumable_Implementation(TSubclassOf<AISItemBase> ItemClass)
{
	if(ISConsumable) return;
	ISConsumable = NewObject<AISConsumable>(GetOwner(),ItemClass);
	ISConsumable->UseItem(GetOwner(),SourceASC);
}

void UISEquipmentComponent::UnUseConsumable_Implementation()
{
	if(ISConsumable) {ISConsumable = nullptr;}
}
//装备防具
void UISEquipmentComponent::EquipGear_Implementation(const FItemInformation TargetInformation)
{
	if(TargetInformation.ArmorType==EArmorType::Helmet)
	{
		if(!ISHelmet)
		{
			ISHelmet = NewObject<AISGear>(GetOwner(),TargetInformation.ItemClassRef);
			ISHelmet->UseItem(GetOwner(),SourceASC);
		}
	}
	else if(TargetInformation.ArmorType == EArmorType::Chest)
	{
		if(!ISChest)
		{
			ISChest = NewObject<AISGear>(GetOwner(),TargetInformation.ItemClassRef);
			ISChest->UseItem(GetOwner(),SourceASC);
		}
	}
	else if(TargetInformation.ArmorType == EArmorType::Pants)
	{
		if(!ISPants)
		{
			ISPants = NewObject<AISGear>(GetOwner(),TargetInformation.ItemClassRef);
			ISPants->UseItem(GetOwner(),SourceASC);
		}
	}
	else
	{
		if(!ISBoots)
		{
			ISBoots = NewObject<AISGear>(GetOwner(),TargetInformation.ItemClassRef);
			ISBoots->UseItem(GetOwner(),SourceASC);
		}
	}
}

void UISEquipmentComponent::UnEquipGear_Implementation(const FItemInformation TargetInformation)
{
	if(TargetInformation.ArmorType==EArmorType::Helmet)
	{
		if(ISHelmet)
		{
			ISHelmet->UnUseItem(GetOwner(),SourceASC);
			ISHelmet = nullptr;
		}
	}
	else if(TargetInformation.ArmorType == EArmorType::Chest)
	{
		if(ISChest)
		{
			ISChest->UnUseItem(GetOwner(),SourceASC);
			ISChest = nullptr;
		}
	}
	else if(TargetInformation.ArmorType == EArmorType::Pants)
	{
		if(ISPants)
		{
			ISPants->UnUseItem(GetOwner(),SourceASC);
			ISPants = nullptr;
		}
	}
	else
	{
		if(ISBoots)
		{
			ISBoots->UnUseItem(GetOwner(),SourceASC);
			ISBoots = nullptr;
		}
	}
}
