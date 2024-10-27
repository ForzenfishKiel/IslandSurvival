// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISGameplayTagsManager.h"

#include "GameplayTagsManager.h"


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

	GameplayTags.Input_Attack_Roll = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Attack.Roll"),FString("翻滚标签"));
	GameplayTags.Input_Attack_FastRun = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Attack.FastRun"),FString("疾跑标签"));
	GameplayTags.Input_Attack_LMB = 
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Attack.LMB"),FString("左键触发标签"));
}