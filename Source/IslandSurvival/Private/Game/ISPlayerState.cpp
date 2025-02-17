// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISAttributeSet.h"

AISPlayerState::AISPlayerState()
{
	NetUpdateFrequency = 100.f;  //网络更新速率
	MinNetUpdateFrequency = 66.f;
	ISAbilitySystemComponent = CreateDefaultSubobject<UISAbilitySystemComponent>(TEXT("ISAbilitySystemComponent"));
	ISAbilitySystemComponent->SetIsReplicated(true);  //设置复制行为为可复制
	ISAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);  //设置ASC仅仅被复制到拥有者的客户端
	ISAttributeSet = CreateDefaultSubobject<UISAttributeSet>(TEXT("ISAttributeSet"));

}

UAbilitySystemComponent* AISPlayerState::GetAbilitySystemComponent() const
{
	return ISAbilitySystemComponent;
}

void AISPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AISPlayerState,CurrentLevel);
	DOREPLIFETIME(AISPlayerState, CurrentXP);
	DOREPLIFETIME(AISPlayerState, AttributePoint);
}

void AISPlayerState::AddToLevel(int32 InLevel)
{
	CurrentLevel+=InLevel;
	OnPlayerLevelChanged.Broadcast(CurrentLevel);
}

void AISPlayerState::SetLevel(int32 InLevel)
{
	CurrentLevel = InLevel;
	OnPlayerLevelChanged.Broadcast(CurrentLevel);
}
void AISPlayerState::AddToXP(int32 InXP)
{
	CurrentXP+=InXP;
	OnPlayerXPChange.Broadcast(CurrentXP);
}
void AISPlayerState::SetXP(int32 InXP)
{
	CurrentXP = InXP;
	OnPlayerXPChange.Broadcast(CurrentXP);
}

void AISPlayerState::AddToAttributePoint(int32 InAttributePoint)
{
	AttributePoint+=InAttributePoint;
	OnPlayerAttributePointChange.Broadcast(AttributePoint);
}

void AISPlayerState::SetAttributePoint(int32 InAttributePoint)
{
	
}

//设置玩家的姓名
void AISPlayerState::SetSourcePlayerName(const FString& InSourcePlayerName)
{
	PlayerName = InSourcePlayerName;
}

//向作为属性一部分的对应属性加点的值进行添加
void AISPlayerState::AddTargetAttributeLevel(const FGameplayAttribute TargetPointType)
{
	if(AttributePoint>=1)
	{
		UGameplayEffect*Effect = NewObject<UGameplayEffect>(GetTransientPackage(),FName("CraftingXP")); //创建一个临时的GE变量
		Effect->DurationPolicy = EGameplayEffectDurationType::Instant;  //顺时地传输
		Effect->PeriodicInhibitionPolicy = EGameplayEffectPeriodInhibitionRemovedPolicy::NeverReset;//设置每次应用不会重置触发时间
		Effect->StackingType = EGameplayEffectStackingType::AggregateBySource; //聚合为原自身
		Effect->StackLimitCount = 1;  //设置可堆的栈上限为1，防止栈溢出
		Effect->StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::ClearEntireStack;  //当应用效果完毕后，栈会清除

		UTargetTagsGameplayEffectComponent&TargetTagsGameplayEffectComponent = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
		FInheritedTagContainer InheritedTagContainer = TargetTagsGameplayEffectComponent.GetConfiguredTargetTagChanges();
		TargetTagsGameplayEffectComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

		const int32 Index = Effect->Modifiers.Num();  //获取当前修改属性Modifiers的长度，也就是最新的那一位
		Effect->Modifiers.Add(FGameplayModifierInfo());
		FGameplayModifierInfo&ModifierInfo = Effect->Modifiers[Index];  //通过下标索引获取对应的Modifier
		ModifierInfo.ModifierMagnitude = FScalableFloat(1.f);
		ModifierInfo.ModifierOp = EGameplayModOp::Additive;  //设置属性运算符号
		ModifierInfo.Attribute = TargetPointType;  //指定对应的属性


		FGameplayEffectContextHandle GameplayEffectContextHandle = ISAbilitySystemComponent->MakeEffectContext();
		GameplayEffectContextHandle.AddSourceObject(this); //添加效果的来源为自身
		if(const FGameplayEffectSpec*MutableSpec = new FGameplayEffectSpec(Effect,GameplayEffectContextHandle,1.f))
		{
			ISAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*MutableSpec);  //应用效果
			AttributePoint-=1;
			OnPlayerAttributePointChange.Broadcast(AttributePoint);  //往UI更新属性点
			return;
		}
	}
}



void AISPlayerState::OnRep_Level(int32 OldLevel) const
{
	OnPlayerLevelChanged.Broadcast(CurrentLevel);   //服务器的属性更改后，复制到客户端时，调用该委托，将复制的值同步到客户端
}

void AISPlayerState::OnRep_XP(int32 OldXP) const
{
	OnPlayerXPChange.Broadcast(CurrentXP);
}

void AISPlayerState::OnRep_AttributePoints(int32 OldAttributePoints) const
{
	OnPlayerAttributePointChange.Broadcast(AttributePoint);
}
FVector AISPlayerState::GetPlayerRespawnLocation() const
{
	return FVector();  //返回角色重生的位置
}

void AISPlayerState::SetPlayerRespawnLocation(const FVector& InPlayerRespawnLocation)
{
	/*设置角色重生的位置*/
}