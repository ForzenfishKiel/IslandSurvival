// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISGameplayTagsManager.h"

#include "GameplayTagsManager.h"
#include "GameplayTagsSettings.h"


FGameplayTagsManager FGameplayTagsManager::GameplayTags;
void FGameplayTagsManager::InitializeNativeGameplayTags()
{
	GameplayTags.Attribute_Primary_Health =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Health"),FString("生命值标签"));
	GameplayTags.Attribute_Primary_Resilience =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Resilience"),FString("抗性标签"));
	GameplayTags.Attribute_Primary_Speed =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Speed"),FString("速度标签"));
	GameplayTags.Attribute_Primary_Vigor =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Vigor"),FString("体力标签"));
	GameplayTags.Attribute_Primary_BaseAttack =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.BaseAttack"),FString("基础攻击力标签"));
	GameplayTags.Attribute_Primary_Hungry =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Hungry"),FString("饥饿值标签"));
	GameplayTags.Attribute_Primary_Thirsty =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Thirsty"),FString("口渴值标签"));

	
	GameplayTags.Attribute_Secondary_Armor =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor"),FString("护甲标签"));
	GameplayTags.Attribute_Secondary_MaxHealth =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxHealth"),FString("最大生命值标签"));
	GameplayTags.Attribute_Secondary_Attack =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Attack"),FString("攻击力标签"));

	
	GameplayTags.Attribute_Meta_InComingDamage = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Meta.InComingDamage"),FString("收到伤害标签"));
	GameplayTags.Attribute_Meta_InComingRecovery = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Meta.InComingRecovery"),FString("受到治疗标签"));
	GameplayTags.Attribute_Meta_InComingXP = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Meta.InComingXP"),FString("受到经验标签"));

	GameplayTags.State_UnEquip = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.UnEquip"),FString("卸下装备标签"));
	GameplayTags.State_Equip = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Equip"),FString("装备标签"));
	GameplayTags.State_VigorRecover = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.VigorRecover"),FString("体力回复标签"));
	GameplayTags.State_Attacking =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Attacking"),FString("正在攻击状态"));
	GameplayTags.State_Ability_Cooldown =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Ability.Cooldown"),FString("技能冷却状态"));

	GameplayTags.Input_Attack_Roll = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Attack.Roll"),FString("翻滚标签"));
	GameplayTags.Input_Attack_FastRun = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Attack.FastRun"),FString("疾跑标签"));
	GameplayTags.Input_Attack_LMB = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Attack.LMB"),FString("左键触发标签"));
	GameplayTags.Input_Attack_SecondInteract =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Attack.SecondInteract"),FString("次向交互"));
	GameplayTags.Input_Attack_Fire =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Attack.Fire"),FString("角色开火"));
	GameplayTags.Input_Attack_BowShoot =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Attack.BowShoot"),FString("弓箭射击"));

	GameplayTags.Input_Combo_Combo01 = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Combo.Combo01"),FString("连招第一段"));
	GameplayTags.Input_Combo_Combo02 = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Combo.Combo02"),FString("连招第二段"));
	GameplayTags.Input_Combo_Combo03 = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Combo.Combo03"),FString("连招第三段"));
	GameplayTags.Input_Combo_Combo04 = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Combo.Combo04"),FString("连招第四段"));
	GameplayTags.Input_Combo_CanCombo = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Combo.CanCombo"),FString("连击窗口"));

	GameplayTags.Input_State_Collecting =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.State.Collecting"),FString("采集中"));

	GameplayTags.AI_State_Eating =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.State.Eating"),FString("AI吃东西"));
	GameplayTags.AI_State_Dying =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.State.Dying"),FString("AI正在死亡"));
	GameplayTags.AI_State_Attacking =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.State.Attacking"),FString("AI正在攻击"));
	GameplayTags.AI_State_Howl =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.State.Howl"),FString("AI正在嚎叫"));
}