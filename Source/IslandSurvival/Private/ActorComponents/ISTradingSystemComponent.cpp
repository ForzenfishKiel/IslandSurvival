// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISTradingSystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Character/ISCharacter.h"
#include "Game/ISGameInstance.h"
#include "Game/ISGameplayMode.h"
#include "Interface/ISNPCInterface.h"
#include "Kismet/GameplayStatics.h"

void UISTradingSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwner()->HasAuthority())
	{
		LoadingTradBackPack();   //在服务器加载商人的交易背包
	}
}

//加载商人的交易背包，这是一个游戏开始加载和动态加载的功能，因此只需要执行一次
void UISTradingSystemComponent::LoadingTradBackPack()
{
	const AISGameplayMode* ISGameMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(GetOwner()));
	if(!ISGameMode) return;
	const UISGameInstance* ISGameInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetOwner()));  //获取当前关卡的游戏实例
	if(!ISGameInstance) return;

	const UDataTable* ISDataTable = ISGameInstance->ItemDataTable;  //获取游戏的物品表
	if(!ISDataTable) return;

	/*获取商人的可交易表，该表包含交易物品的ID和交易物品的数量*/
	const UISTraderSpecialData* NPCTraderData = UISAbilitysystemLibary::GetTraderSpecialData(GetOwner()); //获取交易表Data
	if(!NPCTraderData) return;
	const FTraderSalesData TradCT = NPCTraderData->GetTraderSalesData(IISNPCInterface::Execute_GetCharacterName(GetOwner()));

	const UCurveTable* NumsTradCT = TradCT.TradNumCurveTable; //获取交易的价格表
	if(!NumsTradCT) return;

	for(const auto& CurveKeys : NumsTradCT->GetCurves())
	{
		const FRealCurve* RealCurve = CurveKeys.CurveToEdit;
		if(!RealCurve) return;

		const float SalesNums = RealCurve->Eval(IISNPCInterface::Execute_GetFavorability(GetOwner()));  //根据好感度获取对应的数量
		const float RandValue = FMath::RandRange(0, 3);
		const float ResultRand = SalesNums - RandValue;
		const float ResultValue = FMath::Clamp(ResultRand,1,SalesNums);

		if(FItemInformation* PlayerItemDataTable = ISDataTable->FindRow<FItemInformation>(CurveKeys.CurveName,TEXT("ID")))
		{
			PlayerItemDataTable->ItemQuantity = ResultValue;
			InventoryContainer.Emplace(*PlayerItemDataTable);  //数组新导入
		}
	}
}

//玩家购买时
void UISTradingSystemComponent::TradBegin_Implementation(AActor* TargetActor)
{
	check(NPCTradInComingCoinEffect);  //断言，交易的对象不能是不可购买的
	AISCharacter* TargetCharacter = IISPlayerInterface::Execute_GetSourceCharacter(TargetActor);  //获取角色
	if(!TargetCharacter) return;  //只有玩家才能交易

	AISPlayerState* PlayerState = IISPlayerInterface::Execute_GetPlayerState(TargetActor);
	if(!PlayerState) return;  //检查玩家的状态

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerState);
	if(!TargetASC) return; //检查玩家的ASC

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());  //获取自身的ASC
	if(!SourceASC) return;

	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(GetOwner());
	FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(NPCTradInComingCoinEffect,1.f,ContextHandle);
	const FActiveGameplayEffectHandle ActivateEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	if(!ActivateEffectHandle.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("无法交易！！"));
	}
}

//角色开始出售
void UISTradingSystemComponent::SaleBegin_Implementation(AActor* TargetActor)
{
	check(NPCTradInRecoverCoinEffect);  //断言，交易的对象不能是不可出售的
	AISCharacter* TargetCharacter = IISPlayerInterface::Execute_GetSourceCharacter(TargetActor);  //获取角色
	if(!TargetCharacter) return;  //只有玩家才能交易

	AISPlayerState* PlayerState = IISPlayerInterface::Execute_GetPlayerState(TargetActor);
	if(!PlayerState) return;  //检查玩家的状态

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerState);
	if(!TargetASC) return; //检查玩家的ASC

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());  //获取自身的ASC
	if(!SourceASC) return;

	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(GetOwner());
	FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(NPCTradInRecoverCoinEffect,1.f,ContextHandle);
	const FActiveGameplayEffectHandle ActivateEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	if(!ActivateEffectHandle.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("无法出售！！"));
	}
}
