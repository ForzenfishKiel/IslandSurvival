// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"

#include "Ability/ISInputAbility.h"
#include "Game/ISGameplayMode.h"
#include "Interface/ISCombatInterface.h"
#include "Kismet/GameplayStatics.h"

UISDropCollectibleDataAsset* UISAbilitysystemLibary::GetDropCollectibleDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode*SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->CollectibleDropDataAsset;
}

UISEquipableDataAsset* UISAbilitysystemLibary::GetEquipableDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode*SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->EquipableDataAsset;
}

UISWeaponDataAsset* UISAbilitysystemLibary::GetSweaponDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode*SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->WeaponDataAsset;
}

UISHarvestDataAsset* UISAbilitysystemLibary::GetHarvestDataAsset(const UObject* WorldContextObject)
{
	AISGameplayMode*SourceGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameMode) return nullptr;
	return SourceGameMode->HarvestDataAsset;
}

UISCharacterClassInfo* UISAbilitysystemLibary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AISGameplayMode* SourceGameplayMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGameplayMode) return nullptr;
	return SourceGameplayMode->ISCharacterClassInfo;
}

void UISAbilitysystemLibary::InitializeCharacterAttributes(const UObject* WorldContextObject, FName InName,
	float Level, UAbilitySystemComponent* ASC)
{
	const AActor* AvatarActor = ASC->GetAvatarActor();

	//从实例获取关卡角色的配置
	UISCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(AvatarActor);
	if(CharacterClassInfo == nullptr) return;

	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassData(InName);

	//设置基础属性
	FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();  //拿到效果的上下文
	PrimaryContextHandle.AddSourceObject(AvatarActor);  //将上下文的源对象应用于ASC获取的对象
	const FGameplayEffectSpecHandle PrimarySpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.DefaultAttributes,Level,PrimaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	//设置次要属性
	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttribute,Level,SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());
}

void UISAbilitysystemLibary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
	FName InName)
{
	UISCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;

	int32 CharacterLevel = 0.f;
	if(ASC->GetAvatarActor()->Implements<UISCombatInterface>())
	{
		CharacterLevel = IISCombatInterface::Execute_GetLevel(ASC->GetAvatarActor());
	}
	const FCharacterClassDefaultInfo CharacterDefaultInfo = CharacterClassInfo->GetCharacterClassData(InName);

	for(const TSubclassOf<UGameplayAbility>& ActivateAbilityRef : CharacterDefaultInfo.StartupActivateAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(ActivateAbilityRef,CharacterLevel);
		ASC->GiveAbility(AbilitySpec);
	}

	for(const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterDefaultInfo.StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CharacterLevel);
		ASC->GiveAbilityAndActivateOnce(AbilitySpec);  //应用并激活一次
	}
}

int32 UISAbilitysystemLibary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, FName EnemyName,
	int32 CharacterLevel)
{
	UISCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& CharacterClassDefaultInfo = CharacterClassInfo->GetCharacterClassData(EnemyName);

	const float XPReward = CharacterClassDefaultInfo.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);  //返回查找表的怪物的经验值
}
