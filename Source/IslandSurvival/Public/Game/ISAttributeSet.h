// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "ISAttributeSet.generated.h"
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;  //储存效果应用时的上下文

	UPROPERTY()
	UAbilitySystemComponent*SourceASC = nullptr;
	
	UPROPERTY()
	AActor*SourceAvatarActor = nullptr;

	UPROPERTY()
	AController*SourceController = nullptr;

	UPROPERTY()
	ACharacter*SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent*TargetASC = nullptr;

	UPROPERTY()
	AActor*TargetAvatarActor = nullptr;

	UPROPERTY()
	AController*TargetController = nullptr;

	UPROPERTY()
	ACharacter*TargetCharacter = nullptr;
};
UCLASS()
class ISLANDSURVIVAL_API UISAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UISAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth,Category = "PrimaryAttributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UISAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSpeed, Category = "PrimaryAttributes")
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UISAttributeSet, MaxSpeed)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health,Category = "SecondaryAttributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack,Category = "PrimaryAttributes")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,Attack)

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Vigor,Category = "SecondaryAttributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,Vigor)

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxVigor,Category = "PrimaryAttributes")
	FGameplayAttributeData MaxVigor;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,MaxVigor)

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_WeaponAttack,Category = "PrimaryAttributes")
	FGameplayAttributeData WeaponAttack;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,WeaponAttack);  //武器伤害

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_GatheringDamage,Category = "PrimaryAttributes")
	FGameplayAttributeData GatheringDamage;  //角色本身自带的采集伤害
	ATTRIBUTE_ACCESSORS(UISAttributeSet,GatheringDamage)
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Hunger,Category = "SecondaryAttributes")
	FGameplayAttributeData Hunger; //人物饱食度
	ATTRIBUTE_ACCESSORS(UISAttributeSet,Hunger)
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxHunger,Category = "PrimaryAttributes")
	FGameplayAttributeData MaxHunger;  //饱食度最大值
	ATTRIBUTE_ACCESSORS(UISAttributeSet,MaxHunger)

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Thirst,Category = "SecondaryAttributes")
	FGameplayAttributeData Thirst;  //口渴值
	ATTRIBUTE_ACCESSORS(UISAttributeSet,Thirst);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxThirst,Category = "PrimaryAttributes")
	FGameplayAttributeData MaxThirst;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,MaxThirst);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Armor,Category = "PrimaryAttributes")
	FGameplayAttributeData Armor;  //总护甲
	ATTRIBUTE_ACCESSORS(UISAttributeSet,Armor);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing =OnRep_GearArmor,Category = "SecondaryAttributes")
	FGameplayAttributeData GearArmor;  //装备护甲
	ATTRIBUTE_ACCESSORS(UISAttributeSet,GearArmor);

	/*元属性*/
	UPROPERTY(BlueprintReadOnly,Category = "MetaAttributes")
	FGameplayAttributeData InComingXP;  //角色获得的XP
	ATTRIBUTE_ACCESSORS(UISAttributeSet,InComingXP);

	UPROPERTY(BlueprintReadOnly,Category = "MetaAttributes")
	FGameplayAttributeData InComingDamage; //角色受到的伤害
	ATTRIBUTE_ACCESSORS(UISAttributeSet,InComingDamage);

	/*角色等级*/
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_PlayerLevel,Category = "CharacterLevel")
	FGameplayAttributeData PlayerLevel;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,PlayerLevel);

	/*角色加点属性*/

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxHealthLevel,Category = "AttributeLevel")
	FGameplayAttributeData MaxHealthLevel;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,MaxHealthLevel);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldAttack) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	UFUNCTION()
	void OnRep_MaxVigor(const FGameplayAttributeData& OldMaxVigor) const;
	UFUNCTION()
	void OnRep_WeaponAttack(const FGameplayAttributeData&OldWeaponAttack) const;
	UFUNCTION()
	void OnRep_GatheringDamage(const FGameplayAttributeData&OldGatheringDamage) const;
	UFUNCTION()
	void OnRep_Hunger(const FGameplayAttributeData&OldHunger) const;
	UFUNCTION()
	void OnRep_MaxHunger(const FGameplayAttributeData&OldMaxHunger) const;
	UFUNCTION()
	void OnRep_Thirst(const FGameplayAttributeData&OldThirst) const;
	UFUNCTION()
	void OnRep_MaxThirst(const FGameplayAttributeData&OldMaxThirst) const;
	UFUNCTION()
	void OnRep_PlayerLevel(const FGameplayAttributeData&OldPlayerLevel) const;
	UFUNCTION()
	void OnRep_MaxHealthLevel(const FGameplayAttributeData&OldMaxHealthLevel) const;
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData&OldArmor) const;
	UFUNCTION()
	void OnRep_GearArmor(const FGameplayAttributeData&OldHelmetArmor) const;
	UFUNCTION()
	void OnRep_MaxSpeed(const FGameplayAttributeData&OldMaxSpeed) const;
protected:
	void SetEffectContext(const FGameplayEffectModCallbackData& Data,FEffectProperties&Properties);
	void SendGameplayXP(const FEffectProperties& Props);
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
