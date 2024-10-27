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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth,Category = "SecondaryAttributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UISAttributeSet, MaxHealth)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health,Category = "PrimaryAttributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack,Category = "SecondaryAttributes")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,Attack)

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Vigor,Category = "SecondaryAttributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,Vigor)

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxVigor,Category = "SecondaryAttributes")
	FGameplayAttributeData MaxVigor;
	ATTRIBUTE_ACCESSORS(UISAttributeSet,MaxVigor)


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
protected:
	void SetEffectContext(const FGameplayEffectModCallbackData& Data,FEffectProperties&Properties);
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
