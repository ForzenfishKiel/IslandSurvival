// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ISAbilitySystemComponent.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&); //单播委托，只能绑定一个回调
UCLASS()
class ISLANDSURVIVAL_API UISAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AddCharacterAbility(TArray<TSubclassOf<UGameplayAbility>>&CharacterAbilities);
	void AddCharacterPassiveAbility(TArray<TSubclassOf<UGameplayAbility>>&CharacterAbilities);
	void InputPressedFunc(const FGameplayTag&InputTag);
	void InputHoldFunc(const FGameplayTag&InputTag);
	void InputReleased(const FGameplayTag&InputTag);
	void RemoveCharacterAbility(TArray<TSubclassOf<UGameplayAbility>>&CharacterAbilities);
	void RemoveCharacterAttribute(const TSubclassOf<UGameplayEffect>AbilityToRemoveEffect);
	void ForEachAbilities(const FForEachAbility& ForEachAbilityDelegate);

	//获取技能的标签
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	//获取技能的输入标签
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	//获取技能的状态标签
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec); 
	
	virtual void OnRep_ActivateAbilities() override;
};
