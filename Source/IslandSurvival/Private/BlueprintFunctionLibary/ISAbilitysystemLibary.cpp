// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Ability/ISInputAbility.h"
#include "Game/ISGameInstance.h"
#include "Game/ISGameplayMode.h"
#include "Game/ISGameplayTagsManager.h"
#include "Interface/ISCombatInterface.h"
#include "Interface/ISPlayerInterface.h"
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

UISTraderSpecialData* UISAbilitysystemLibary::GetTraderSpecialData(const UObject* WorldContextObject)
{
	AISGameplayMode* SourceGamemode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!SourceGamemode) return nullptr;
	return SourceGamemode->ISTraderSpecialData;
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

FItemInformation* UISAbilitysystemLibary::GetItemInformation(const UObject* WorldContextObject,const FName InTargetID)
{
	if(!InTargetID.IsValid()) return nullptr;
	UISGameInstance* SourceGameInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)); //获取本地游戏实例
	if(!SourceGameInstance) return nullptr;

	UDataTable* GameItemDataTable = SourceGameInstance->ItemDataTable;  //获取物品制作表
	if(!GameItemDataTable) return nullptr;
	return GameItemDataTable->FindRow<FItemInformation>(InTargetID,TEXT("ID"));
}

//从游戏存档中初始化角色的属性
void UISAbilitysystemLibary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, UISLocalPlayerSaveGame* SaveGame)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	const FGameplayTagsManager& GameplayTags = FGameplayTagsManager::Get();

	//从实例获取到关卡角色的配置
	const UISCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;

	//*********************************初始化主要属性*********************************

	//创建GE的上下文句柄
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AvatarActor);  //效果来源为对方



	if(AvatarActor->Implements<UISPlayerInterface>())
	{
		//*********************************设置主要属性*********************************

		FGameplayEffectContextHandle PimaryContextHandle = ASC->MakeEffectContext();
		PimaryContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(IISPlayerInterface::Execute_GetPrimaryAttributes(AvatarActor), 1.0f, PimaryContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
		//*********************************设置次级属性*********************************
	
		FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
		SecondaryContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(IISPlayerInterface::Execute_GetSecondaryAttributes(AvatarActor), 1.0f, SecondaryContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

	}
	//根据句柄和类创建GE实例，并可以通过句柄找到GE实例
	const FGameplayEffectSpecHandle PrimaryContextHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.0f, EffectContextHandle);

	//通过标签设置GE使用的配置
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryContextHandle, GameplayTags.Attribute_Primary_Health, SaveGame->Health);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryContextHandle, GameplayTags.Attribute_Primary_Vigor, SaveGame->Vigor);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryContextHandle, GameplayTags.Attribute_Primary_Thirsty, SaveGame->Thirsty);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryContextHandle, GameplayTags.Attribute_Primary_Hungry, SaveGame->Hungry);

	//应用GE
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryContextHandle.Data.Get());
}
