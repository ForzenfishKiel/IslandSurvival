// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISTradingSystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueNotifyTypes.h"
#include "ISAbilityTypes.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Character/ISCharacter.h"
#include "Game/ISAttributeSet.h"
#include "Game/ISGameInstance.h"
#include "Game/ISGameplayMode.h"
#include "Interface/ISNPCInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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
	if(Cast<AISCharacter>(GetOwner())) return;  //玩家（暂时）不用加载交易背包
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

//玩家购买时，传入对方对象
void UISTradingSystemComponent::TradBegin_Implementation(AActor* TargetActor, const FName InTargetID, const int32 TargetIndex)
{
	AISCharacter* TargetCharacter = IISPlayerInterface::Execute_GetSourceCharacter(GetOwner());  //获取角色
	if(!TargetCharacter) return;  //只有玩家才能交易
	
	check(NPCTradInComingCoinEffect);  //断言，交易的对象不能是不可购买的
	
	AISPlayerState* PlayerState = IISPlayerInterface::Execute_GetPlayerState(GetOwner());
	if(!PlayerState) return;  //检查玩家的状态

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerState);  //获取自身的ASC
	if(!SourceASC) return; //检查玩家的ASC

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);  //获取对方的ASC
	if(!TargetASC) return;

	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();

	FISGameplayEffectContext* ISEffectContext = static_cast<FISGameplayEffectContext*>(ContextHandle.Get());
	if(!ISEffectContext) return;
	ISEffectContext->SetTargetSaleID(InTargetID);  //上下文里，导入交易的ID
	ISEffectContext->SetTargetBackPackIndex(TargetIndex);
	
	ContextHandle.AddSourceObject(TargetActor);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(NPCTradInComingCoinEffect,1.f,ContextHandle);
	const FActiveGameplayEffectHandle ActivateEffectHandle = SourceASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

//角色开始出售
void UISTradingSystemComponent::SaleBegin_Implementation(AActor* TargetActor, const FName InTargetID, const int32 TargetIndex)
{
	AISCharacter* TargetCharacter = IISPlayerInterface::Execute_GetSourceCharacter(GetOwner());  //获取角色
	if(!TargetCharacter) return;  //只有玩家才能交易
	
	check(NPCTradInRecoverCoinEffect);  //断言，交易的对象不能是不可出售的
	
	if(!CheckIsCanBeSale(TargetActor,InTargetID,TargetIndex)) return;  //如果无法出售（对方不购买自己的物品）则当前无法出售

	AISPlayerState* PlayerState = IISPlayerInterface::Execute_GetPlayerState(GetOwner());
	if(!PlayerState) return;  //检查玩家的状态

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!TargetASC) return; //检查对方的ASC

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerState);  //获取自身的ASC
	if(!SourceASC) return;  //检查玩家的ASC

	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();

	FISGameplayEffectContext* ISEffectContext = static_cast<FISGameplayEffectContext*>(ContextHandle.Get());
	if(!ISEffectContext) return;
	ISEffectContext->SetTargetSaleID(InTargetID);
	ISEffectContext->SetTargetBackPackIndex(TargetIndex);
	
	ContextHandle.AddSourceObject(TargetActor); //添加效果来源为对方
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(NPCTradInRecoverCoinEffect,1.f,ContextHandle);
	const FActiveGameplayEffectHandle ActivateEffectHandle = SourceASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

bool UISTradingSystemComponent::CheckIsCanBeSale(AActor* TargetActor, FName InTargetID, int32 TargetIndex)
{
	const UISTraderSpecialData* TraderSpecialData = UISAbilitysystemLibary::GetTraderSpecialData(TargetActor);
	if(!TraderSpecialData) return false;

	const FTraderSalesData TraderSalesData = TraderSpecialData->GetTraderSalesData(IISNPCInterface::Execute_GetCharacterName(TargetActor));
	const UCurveTable* SalesCT = TraderSalesData.CanTradeCurveTable;
	if(!SalesCT) return false;  //如果价格表为空直接退回
	const FRealCurve* SalseRealCT = SalesCT->FindCurve(InTargetID,FString());  //获取对应的可出售的ID
	if(SalseRealCT) return true;
	return false;
}

void UISTradingSystemComponent::SetTradTarget_Implementation(const FItemInformation TargetItem, const int32 TargetNums)
{
	
	MARK_PROPERTY_DIRTY_FROM_NAME(UISTradingSystemComponent, TradTarget, this);
	TradTarget = TargetItem;
	
	TargetCoins = TargetNums;
	
	OnTradingSucceeded.Broadcast(TargetItem,TargetNums);  //服务器发送一份
	const FLatentActionInfo LatentInfo(0, FMath::Rand(), TEXT("OnRep_TraCoins"), this);
	UKismetSystemLibrary::Delay(this,0.1f,LatentInfo);
}

void UISTradingSystemComponent::OnRep_TraCoins_Implementation()
{
	OnTradingSucceeded.Broadcast(TradTarget,TargetCoins);
	
}


void UISTradingSystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.Condition = COND_Dynamic;
	SharedParams.bIsPushBased = true;
	SharedParams.RepNotifyCondition = REPNOTIFY_Always;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(UISTradingSystemComponent,TradTarget,SharedParams);
	DOREPLIFETIME_CONDITION_NOTIFY(UISTradingSystemComponent, TargetCoins, COND_Dynamic, REPNOTIFY_Always);
}

