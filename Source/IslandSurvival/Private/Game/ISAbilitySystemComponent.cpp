// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISAbilitySystemComponent.h"

#include "Ability/ISInputAbility.h"
#include "Character/ISCharacter.h"
#include "Game/ISGameplayTagsManager.h"
#include "GameFramework/CharacterMovementComponent.h"

void UISAbilitySystemComponent::AddCharacterAbility(TArray<TSubclassOf<UGameplayAbility>>& CharacterAbilities)
{
	if(CharacterAbilities.Num() > 0)
	{
		for(const TSubclassOf<UGameplayAbility>&AbilityClass:CharacterAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1.f);
			if(const UISInputAbility* InputAbility = Cast<UISInputAbility>(AbilitySpec.Ability))
			{
				AbilitySpec.DynamicAbilityTags.AddTag(InputAbility->InputTag);
				GiveAbility(AbilitySpec);  //给予对象ASC一个可激活的能力
			}
		}
	}
}

void UISAbilitySystemComponent::InputPressedFunc(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;
	FGameplayTagsManager GameplayTagsManager = FGameplayTagsManager::Get();  //从Tag库中获取Tag
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for(FGameplayAbilitySpec&AbilitySpec:GetActivatableAbilities())  //从所有可激活的能力中查找
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if(InputTag==GameplayTagsManager.Input_Attack_LMB&&!Cast<AISCharacter>(GetAvatarActor())->GetCharacterMovement()->IsFalling())
			{
				if(!AbilitySpec.IsActive())
				{
					TryActivateAbility(AbilitySpec.Handle);  //尝试启动
				}
			}
		}
	}
}

void UISAbilitySystemComponent::RemoveCharacterAbility(TArray<TSubclassOf<UGameplayAbility>>& CharacterAbilities)
{
	if(CharacterAbilities.Num() > 0)
	{
		for(auto&Ability:CharacterAbilities)
		{
			for(int32 index = 0;index<GetActivatableAbilities().Num();index++)
			{
				if(GetActivatableAbilities()[index].Ability.GetClass()==Ability)
				{
					ClearAbility(GetActivatableAbilities()[index].Handle);  //删除对应激活的能力
				}
			}
		}
	}
}

void UISAbilitySystemComponent::RemoveCharacterAttribute(const TSubclassOf<UGameplayEffect> AbilityToRemoveEffect)
{
	RemoveActiveGameplayEffectBySourceEffect(AbilityToRemoveEffect,this,1);  //移除一个单位的属性
}
