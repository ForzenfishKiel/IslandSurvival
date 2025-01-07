// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISPlayerItemDataTable.h"
#include "Interface/ISEquipableInterface.h"
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
	EquipRifle = 4 UMETA(DisplayName = "EquipRifle"),
	EquipRPG = 5 UMETA(DisplayName = "EquipRPG"),
};
UCLASS()
class ISLANDSURVIVAL_API AISEquipable : public AISItemBase,public IISEquipableInterface
{
	GENERATED_BODY()
public:
	USceneComponent*GetAttachTarget(APawn*TargetPawn) const;
	USceneComponent* GetAttachThirdPersonParent(APawn*TargetPawn) const;
	UPROPERTY(EditDefaultsOnly,Category="ItemConfig")
	ECharacterEquipState EquipState = ECharacterEquipState::None;
	UPROPERTY(EditDefaultsOnly,Category="ItemConfig")
	EItemRarity ItemRarity = EItemRarity::None;
	void SetEquipableCollision();  //设置武器的碰撞条件
	void InitializeEquipableConfig(const FItemInformation&TargetInfo);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ItemConfig")
	TSubclassOf<UGameplayEffect>EquipableDefaultAttribute;  //武器默认属性的提供
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ItemConfig")
	TArray<UAnimMontage*>EquipableActivateAnimations; //武器调用的攻击动画
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ItemConfig")
	TArray<TSubclassOf<UGameplayAbility>>EquipableActivateAbilities;  //武器主动能力
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ItemConfig")
	TArray<TSubclassOf<UGameplayAbility>>EquibablePassiveAbilities;//武器被动能力

	UPROPERTY(EditAnywhere,BlueprintReadWrite,ReplicatedUsing = OnRep_AmmoChanged,Category="ItemConfig")
	int32 Ammos = 0;  //弹药
	UFUNCTION()
	void OnRep_AmmoChanged();
	UFUNCTION(BlueprintCallable)
	int32 CheckMagazine(const int32 TargetID); //检查是否有弹匣并返回指定位置

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ItemConfig")
	int32 MaxAmmos = 0; //最大弹药量
public:
	virtual void UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) override;
	virtual void UnUseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) override;
	virtual void ApplyEffectToTarget(UAbilitySystemComponent* InASC, TSubclassOf<UGameplayEffect> EffectClass) override;
	virtual void AddTargetAbility(UAbilitySystemComponent* TargetASC, TArray<TSubclassOf<UGameplayAbility>>& TargetArray) override;
	virtual void AddTargetPassiveAbility(UAbilitySystemComponent* TargetASC, TArray<TSubclassOf<UGameplayAbility>>& TargetArray) override;
	virtual void RemoveTargetAbility(UAbilitySystemComponent* TargetASC, TArray<TSubclassOf<UGameplayAbility>>& TargetArray) override;
	virtual void RemoveTargetEffect(UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> EffectClass) override;
	virtual ECharacterEquipState GetTargetEquipState() override;
	virtual EItemRarity GetItemRarity_Implementation() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
