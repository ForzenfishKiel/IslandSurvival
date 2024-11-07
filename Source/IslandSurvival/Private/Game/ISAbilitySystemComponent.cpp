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
			//左键触发
			if(InputTag==GameplayTagsManager.Input_Attack_LMB&&!Cast<AISCharacter>(GetAvatarActor())->GetCharacterMovement()->IsFalling())
			{
				//第一次攻击触发
				if(!HasMatchingGameplayTag(GameplayTagsManager.State_Attacking)&&
					AbilitySpec.Ability->AbilityTags.HasTagExact(GameplayTagsManager.Input_Combo_Combo01)&&!AbilitySpec.IsActive())
				{
					TryActivateAbility(AbilitySpec.Handle);
				}
				else if(HasMatchingGameplayTag(AbilitySpec.Ability->AbilityTags.First()))
				{
					TryActivateAbility(AbilitySpec.Handle);
				}
			}
			else if(InputTag==GameplayTagsManager.Input_State_Collecting&&!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
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

void UISAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
}
