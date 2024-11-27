// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ISItemBase.h"
#include "ISGear.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISGear : public AISItemBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TArray<TSubclassOf<UGameplayAbility>>GearPassiveAbilities;  //装备会附着的被动技能
public:
	virtual void UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) override;
	virtual void UnUseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) override;
	virtual void ApplyEffectToTarget(UAbilitySystemComponent* InASC, TSubclassOf<UGameplayEffect> EffectClass) override;  //添加应用效果
	virtual void AddTargetAbility(UAbilitySystemComponent* TargetASC, TArray<TSubclassOf<UGameplayAbility>>& TargetArray) override;  //添加应用技能
	virtual void AddTargetPassiveAbility(UAbilitySystemComponent* TargetASC, TArray<TSubclassOf<UGameplayAbility>>& TargetArray) override;
	virtual void RemoveTargetAbility(UAbilitySystemComponent* TargetASC, TArray<TSubclassOf<UGameplayAbility>>& TargetArray) override;  //移除技能
	virtual void RemoveTargetEffect(UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> EffectClass) override;//移除效果
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TSubclassOf<UGameplayEffect> GearDefaultEffect; //装备会改变的属性
};
