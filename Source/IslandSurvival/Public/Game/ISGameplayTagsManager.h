// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
struct FGameplayTagsManager
{
public:
	static const FGameplayTagsManager&Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();
	/*主要属性传递*/
	FGameplayTag Attribute_Primary_Health;
	FGameplayTag Attribute_Primary_BaseAttack;
	FGameplayTag Attribute_Primary_Vigor;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Speed;
	/*主要属性创建结束*/

	
	/*次要属性创建开始*/
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_Attack;
	/*次要属性创建结束*/

	/*元属性*/
	FGameplayTag Attribute_Meta_InComingDamage;
	FGameplayTag Attribute_Meta_InComingRecovery;
	FGameplayTag Attribute_Meta_InComingXP;
	/*元属性结束*/


	/*输入事件*/
	FGameplayTag Input_Attack_LMB;
	FGameplayTag Input_Attack_Roll;
	FGameplayTag Input_Attack_FastRun;

	/*状态相关*/
	FGameplayTag State_Equip;
	FGameplayTag State_UnEquip;
	FGameplayTag State_VigorRecover;
private:
	static FGameplayTagsManager GameplayTags;
};