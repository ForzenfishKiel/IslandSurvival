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
	FGameplayTag Input_Attack_SecondInteract;
	FGameplayTag Input_Attack_Fire; //角色开火
	FGameplayTag Input_Attack_BowShoot;  //弓箭射击
	/*输入事件结束*/

	/*输入触发状态事件*/
	FGameplayTag Input_State_Collecting;  //采集中

	/*状态相关*/
	FGameplayTag State_Attacking;
	FGameplayTag State_Equip;
	FGameplayTag State_UnEquip;
	FGameplayTag State_VigorRecover;

	/*连招判定*/
	FGameplayTag Input_Combo_Combo01;
	FGameplayTag Input_Combo_Combo02;
	FGameplayTag Input_Combo_Combo03;
	FGameplayTag Input_Combo_Combo04;
	FGameplayTag Input_Combo_CanCombo;

	/*AI状态*/
	FGameplayTag AI_State_Eating;
private:
	static FGameplayTagsManager GameplayTags;
};