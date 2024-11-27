// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ISGear.h"

#include "Game/ISAbilitySystemComponent.h"

void AISGear::UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC)
{
	Super::UseItem(TargetCharacter, TargetASC);
	ApplyEffectToTarget(TargetASC,GearDefaultEffect);  //传入属性更改
	AddTargetPassiveAbility(TargetASC,GearPassiveAbilities);  //传入被动技能
}

void AISGear::UnUseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC)
{
	Super::UnUseItem(TargetCharacter, TargetASC);
	RemoveTargetEffect(TargetASC,GearDefaultEffect);  //清除目标属性更改
	RemoveTargetAbility(TargetASC,GearPassiveAbilities);  //清除被动技能
}

void AISGear::RemoveTargetAbility(UAbilitySystemComponent* TargetASC,
	TArray<TSubclassOf<UGameplayAbility>>& TargetArray)
{
	Super::RemoveTargetAbility(TargetASC, TargetArray);
	UISAbilitySystemComponent*SourceASC = Cast<UISAbilitySystemComponent>(TargetASC);
	if(!SourceASC) return;
	SourceASC->RemoveCharacterAbility(TargetArray); //调用本地ASC清除角色被动技能组
}

void AISGear::RemoveTargetEffect(UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> EffectClass)
{
	Super::RemoveTargetEffect(TargetASC, EffectClass);
	UISAbilitySystemComponent*SourceASC = Cast<UISAbilitySystemComponent>(TargetASC);
	if(!SourceASC&&!IsValid(EffectClass)) return;
	SourceASC->RemoveCharacterAttribute(EffectClass);  //调用本地ASC清除角色属性效果更改
}

void AISGear::ApplyEffectToTarget(UAbilitySystemComponent* InASC, TSubclassOf<UGameplayEffect> EffectClass)
{
	Super::ApplyEffectToTarget(InASC, EffectClass);
	check(EffectClass);
	FGameplayEffectContextHandle GameplayEffectContext = InASC->MakeEffectContext();
	GameplayEffectContext.AddSourceObject(this);  //添加效果来源为装备
	FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(EffectClass,1.f,GameplayEffectContext);
	InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void AISGear::AddTargetAbility(UAbilitySystemComponent* TargetASC, TArray<TSubclassOf<UGameplayAbility>>& TargetArray)
{
	Super::AddTargetAbility(TargetASC, TargetArray);
}

void AISGear::AddTargetPassiveAbility(UAbilitySystemComponent* TargetASC,
	TArray<TSubclassOf<UGameplayAbility>>& TargetArray)
{
	Super::AddTargetPassiveAbility(TargetASC, TargetArray);
	UISAbilitySystemComponent*SourceASC = Cast<UISAbilitySystemComponent>(TargetASC);
	if(!SourceASC) return;
	SourceASC->AddCharacterPassiveAbility(TargetArray);  //调用本地ASC添加角色被动技能
}
