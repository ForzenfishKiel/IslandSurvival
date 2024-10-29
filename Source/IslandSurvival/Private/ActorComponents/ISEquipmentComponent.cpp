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
}

void UISEquipmentComponent::Equip_Implementation(TSubclassOf<AISItemBase> InTargetItem)
{
	//确定储存的武器数据不为空
	Equipable = NewObject<AISEquipable>(GetOwner(),InTargetItem);
	CharacterEquipState = Equipable->CurrentEquipState;
	Equipable->UseItem(GetOwner(),SourceASC);
	Equipable = GetWorld()->SpawnActorDeferred<AISEquipable>(InTargetItem,FTransform::Identity,GetOwner());
	UGameplayStatics::FinishSpawningActor(Equipable,FTransform::Identity);
	USceneComponent*AttachTarget = Equipable->GetAttachTarget(Cast<APawn>(Equipable->GetOwner()));
	USceneComponent*AttachThirdPerson = Equipable->GetAttachThirdPersonParent(Cast<APawn>(Equipable->GetOwner()));
	SpawnEquip_Implementation(AttachThirdPerson);
	SpawnEquip_Implementation(AttachTarget);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Owner: %s"), *Equipable->GetOwner()->GetName()));
}

void UISEquipmentComponent::SpawnEquip_Implementation(USceneComponent* AttachEquip)
{
	Equipable->SetEquipableCollision();
	Equipable->SetActorRelativeTransform(FTransform::Identity);
	Equipable->AttachToComponent(AttachEquip,FAttachmentTransformRules::KeepRelativeTransform,FName("WEAPON_R"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("多播你冯！！"));
}

void UISEquipmentComponent::UnEquip_Implementation()
{
	CharacterEquipState = ECharacterEquipState::None;
	Equipable->UnUseItem(GetOwner(),SourceASC);
	Equipable->Destroy();
	Equipable = nullptr;
}