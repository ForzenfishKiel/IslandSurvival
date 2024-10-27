// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISEquipmentComponent.h"
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

void UISEquipmentComponent::Equip_Implementation(TSubclassOf<AISItemBase> InTargetItem)
{
	//确定储存的武器数据不为空
	if(Equipable!=nullptr)
	{
		Equipable = NewObject<AISEquipable>(GetOwner(),InTargetItem);
		CharacterEquipState = Equipable->CurrentEquipState;
		Equipable->UseItem(GetOwner(),SourceASC);
		Equipable = GetWorld()->SpawnActorDeferred<AISEquipable>(InTargetItem,FTransform::Identity,GetOwner());
		UGameplayStatics::FinishSpawningActor(Equipable,FTransform::Identity);
		USceneComponent*AttachTarget = Equipable->GetAttachTarget(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetPawn());
		if(!AttachTarget) return;
		Equipable->SetActorRelativeTransform(FTransform::Identity);
		Equipable->AttachToComponent(AttachTarget,FAttachmentTransformRules::KeepRelativeTransform,FName("WEAPON_R"));
	}
}

void UISEquipmentComponent::UnEquip_Implementation()
{
	if(Equipable==nullptr) return;  //本来就是空的话就跳过该进程
	CharacterEquipState = ECharacterEquipState::None;
	Equipable->UnUseItem(GetOwner(),SourceASC);
	Equipable->Destroy();
	Equipable = nullptr;
}

