// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ISItemBase.h"
#include "ISEquipable.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECharacterEquipState:uint8
{
	None = 0 UMETA(DisplayName = "None"),
	EquipAxe = 1 UMETA(DisplayName = "EquipAxe"),
	EquipSword = 2 UMETA(DisplayName = "EquipSword"),
	EquipBow = 3 UMETA(DisplayName = "EquipBow"),
};
UCLASS()
class ISLANDSURVIVAL_API AISEquipable : public AISItemBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="EquipState")
	ECharacterEquipState CurrentEquipState = ECharacterEquipState::None;  //装备武器类型
	USceneComponent*GetAttachTarget(APawn*TargetPawn) const;
	USceneComponent* GetAttachThirdPersonParent(APawn*TargetPawn) const;
	void SetEquipableCollision();  //设置武器的碰撞条件


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="EquipState")
	TSubclassOf<UGameplayEffect>EquipableDefaultAttribute;  //武器默认属性的提供
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	TArray<UAnimMontage*>EquipableActivateAnimations; //武器调用的攻击动画
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Ability")
	TArray<TSubclassOf<UGameplayAbility>>EquipableActivateAbilities;  //武器主动能力
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Ability")
	TArray<TSubclassOf<UGameplayAbility>>EquibablePassiveAbilities;//武器被动能力
public:
	virtual void UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) override;
	virtual void UnUseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) override;
	virtual void ApplyEffectToTarget(UAbilitySystemComponent* InASC, TSubclassOf<UGameplayEffect> EffectClass) override;
	virtual void AddTargetAbility(UAbilitySystemComponent* TargetASC, TArray<TSubclassOf<UGameplayAbility>>& TargetArray) override;
	virtual void RemoveTargetAbility(UAbilitySystemComponent* TargetASC, TArray<TSubclassOf<UGameplayAbility>>& TargetArray) override;
	virtual void RemoveTargetEffect(UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> EffectClass) override;
};
