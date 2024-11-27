// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ISEquipable.h"
#include "Character/ISCharacter.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Game/ISGameplayTagsManager.h"

USceneComponent* AISEquipable::GetAttachTarget(APawn* TargetPawn) const
{
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
