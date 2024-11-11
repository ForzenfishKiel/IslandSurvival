// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISEquipmentComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UISEquipmentComponent::UISEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UISEquipmentComponent::InitializeEquipmentComponent(UAbilitySystemComponent*TargetASC)
{
	SourceASC = TargetASC;
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
	SpawnEquip_Implementation(AttachThirdPerson);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Owner: %s"), *Equipable->GetOwner()->GetName()));
}

void UISEquipmentComponent::SpawnEquip_Implementation(USceneComponent* AttachEquip)
{
	Equipable->SetEquipableCollision();
	Equipable->ItemsStaticMesh->bOwnerNoSee = true;
	Equipable->SetActorRelativeTransform(FTransform::Identity);
	Equipable->AttachToComponent(AttachEquip,FAttachmentTransformRules::KeepRelativeTransform,FName("WEAPON_R"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("多播触发！！"));
}

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

