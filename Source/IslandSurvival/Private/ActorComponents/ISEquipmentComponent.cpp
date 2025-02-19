// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISEquipmentComponent.h"

#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Character/ISCharacter.h"
#include "DataAsset/ISWeaponDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UISEquipmentComponent::UISEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true); //默认打开复制
}

//Setter
void UISEquipmentComponent::SetTPAttachSocket(FName NewSocketName)
{
	Tp_AttachSocket = NewSocketName;
}

void UISEquipmentComponent::SetFPAttachSocket(FName NewSocketName)
{
	Fp_AttachSocket = NewSocketName;
}

//Setter

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

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	//SharedParams.Condition = ELifetimeCondition::COND_AutonomousOnly;

	FDoRepLifetimeParams SharedParams2;
	SharedParams2.bIsPushBased = true;
	//SharedParams2.Condition = ELifetimeCondition::COND_SkipOwner;
	
	DOREPLIFETIME(UISEquipmentComponent,CharacterEquipState);
	DOREPLIFETIME(UISEquipmentComponent,ISHelmet);
	DOREPLIFETIME(UISEquipmentComponent,ISChest);
	DOREPLIFETIME(UISEquipmentComponent,ISPants);
	DOREPLIFETIME(UISEquipmentComponent,ISBoots);
	DOREPLIFETIME(UISEquipmentComponent,Tp_AttachSocket);
	DOREPLIFETIME(UISEquipmentComponent,Fp_AttachSocket);

	
	DOREPLIFETIME_WITH_PARAMS_FAST(UISEquipmentComponent,Equipable,SharedParams);
}


//在服务器完成武器的初始化
void UISEquipmentComponent::Equip_Implementation(const FItemInformation TargetInformation)
{
	UISWeaponDataAsset* SourceWeaponData = UISAbilitysystemLibary::GetSweaponDataAsset(GetOwner());
	if(!SourceWeaponData) return;
	FWeaponInfo WeaponInfo = SourceWeaponData->GetWeaponInfo(TargetInformation.ItemID);
	
	SetTPAttachSocket(WeaponInfo.Tp_AttachSocket);
	SetFPAttachSocket(WeaponInfo.Fp_AttachSocket);
	
	EquipableTP = GetWorld()->SpawnActorDeferred<AISEquipable>(TargetInformation.ItemClassRef,FTransform::Identity,GetOwner(),GetOwner()->GetInstigatorController()->GetPawn());
	UGameplayStatics::FinishSpawningActor(EquipableTP,FTransform::Identity);
	
	//确定储存的武器数据不为空
	Equipable = GetWorld()->SpawnActorDeferred<AISEquipable>(TargetInformation.ItemClassRef,FTransform::Identity,GetOwner(),GetOwner()->GetInstigatorController()->GetPawn());
	UGameplayStatics::FinishSpawningActor(Equipable,FTransform::Identity);



	Equipable->UseItem(GetOwner(),SourceASC);  //角色第一人称武器装备时，只需要应用以第一人称装备为标准就行

	
	Equipable->InitializeEquipableConfig(TargetInformation);  //初始化其配置
	CharacterEquipState = Equipable->EquipState;
	OnRep_Equipable();

	USceneComponent* AttachThirdPerson = EquipableTP->GetAttachThirdPersonParent(Cast<APawn>(Equipable->GetOwner()));  //获取第三人称装备套接字
	
	EquipableTP->SetEquipableCollision();


	if(!EquipableTP) return;
	EquipableTP->SetActorRelativeTransform(FTransform::Identity);
	EquipableTP->AttachToComponent(AttachThirdPerson,FAttachmentTransformRules::KeepRelativeTransform,Tp_AttachSocket);
	
	//SpawnEquip(AttachThirdPerson);  //多播装备
}

// NetMulticast
void UISEquipmentComponent::SpawnEquip_Implementation(USceneComponent* AttachEquip)
{
	//将第三人称的武器装备在第三人称人物身上
	if(!EquipableTP) return;
	EquipableTP->SetActorRelativeTransform(FTransform::Identity);
	EquipableTP->AttachToComponent(AttachEquip,FAttachmentTransformRules::KeepRelativeTransform,Tp_AttachSocket);
}

void UISEquipmentComponent::OnRep_Equipable()
{
	if(Equipable)
	{
		SpawnEquipOnClient();
	}
}

void UISEquipmentComponent::OnRep_EquipableTP()
{
	
}

//客户端调用武器装备
void UISEquipmentComponent::SpawnEquipOnClient_Implementation()
{
	if(!Equipable) return;
	USceneComponent*AttachFirstPerson = Equipable->GetAttachTarget(Cast<APawn>(Equipable->GetOwner()));
	if(!AttachFirstPerson) return;
	Equipable->SetActorRelativeTransform(FTransform::Identity);
	Equipable->SetEquipableCollision();
	Equipable->AttachToComponent(AttachFirstPerson,FAttachmentTransformRules::KeepRelativeTransform,Fp_AttachSocket);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("客户端触发！！！"));
}

//UnEquip Run On Server
void UISEquipmentComponent::UnEquip_Implementation()
{
	CharacterEquipState = ECharacterEquipState::None;
	Equipable->UnUseItem(GetOwner(),SourceASC);
	Equipable->Destroy();
	Equipable = nullptr;
	EquipableTP->Destroy();
	EquipableTP = nullptr;
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
	if(TargetInformation.ArmorType == EArmorType::Helmet)
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
