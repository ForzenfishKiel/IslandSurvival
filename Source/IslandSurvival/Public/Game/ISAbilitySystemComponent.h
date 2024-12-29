// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ISAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AddCharacterAbility(TArray<TSubclassOf<UGameplayAbility>>&CharacterAbilities);
	void AddCharacterPassiveAbility(TArray<TSubclassOf<UGameplayAbility>>&CharacterAbilities);
	void InputPressedFunc(const FGameplayTag&InputTag);
	void InputHoldFunc(const FGameplayTag&InputTag);
	void RemoveCharacterAbility(TArray<TSubclassOf<UGameplayAbility>>&CharacterAbilities);
	void RemoveCharacterAttribute(const TSubclassOf<UGameplayEffect>AbilityToRemoveEffect);
	virtual void OnRep_ActivateAbilities() override;
};
