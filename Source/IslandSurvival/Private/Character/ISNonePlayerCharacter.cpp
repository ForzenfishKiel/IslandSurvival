// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISNonePlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/ISCharacter.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISAttributeSet.h"
#include "Game/ISPlayerController.h"
#include "Interface/ISPlayerInterface.h"
#include "Net/UnrealNetwork.h"

AISNonePlayerCharacter::AISNonePlayerCharacter()
{
	NetUpdateFrequency = 100.f;  //网络更新速率
	MinNetUpdateFrequency = 66.f;
	
	NPCAsc = CreateDefaultSubobject<UISAbilitySystemComponent>(TEXT("NPCAbilitySystemComponent"));
	NPCAsc->SetIsReplicated(true);
	NPCAsc->SetReplicationMode(EGameplayEffectReplicationMode::Full); //最小地去复制npc的一些属性
	NPCTradComponent = CreateDefaultSubobject<UISTradingSystemComponent>(TEXT("TradingSystemComponent"));  //创建可交易允许的组件
}

void AISNonePlayerCharacter::LoadingTradWindow_Implementation()
{
	check(TraderUserWidgetClass);  //检查UI类是否存在
	TraderUserWidget = CreateWidget<UISMenuUIBase>(GetWorld(), TraderUserWidgetClass);//创建交互的UI
	TraderUserWidget->SetWidgetOwner(this);  //创建该UI的拥有者为自身
}

UISMenuUIBase* AISNonePlayerCharacter::GetMenuUI_Implementation() const
{
	return TraderUserWidget;
}

void AISNonePlayerCharacter::BindWidgetController_Implementation(AActor* TargetActor)
{
	IISPublicInterface::BindWidgetController_Implementation(TargetActor);
	BindCharacterAttirbuteChange(TargetActor);
}



AISNonePlayerCharacter* AISNonePlayerCharacter::GetNPC_Implementation()
{
	return this;
}

int32 AISNonePlayerCharacter::GetFavorability_Implementation() const
{
	return Favorability;
}

FName AISNonePlayerCharacter::GetCharacterName_Implementation() const
{
	return CharacterName;
}

void AISNonePlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AISNonePlayerCharacter,Favorability); //标记好感度为可复制
	DOREPLIFETIME(AISNonePlayerCharacter,AIState);  //标记AI状态为可复制
}

//调用NPC的获取其ASC的方法
UAbilitySystemComponent* AISNonePlayerCharacter::GetAbilitySystemComponent() const
{
	return NPCAsc;
}

void AISNonePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	LoadingTradWindow();  //游戏开始时加载用户窗口
}

void AISNonePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetOwner(NewController);
}

void AISNonePlayerCharacter::OnNPCWasInteracted_Implementation(AActor* InteractingActor)
{
	IISNPCInterface::OnNPCWasInteracted_Implementation(InteractingActor);
	Execute_SetAIState(this,Trading);  //设置AI状态为交易
}


void AISNonePlayerCharacter::BindCharacterAttirbuteChange(AActor* TargetActor)
{
	AISCharacter* SourceCharacter = IISPlayerInterface::Execute_GetSourceCharacter(TargetActor);
	if(!SourceCharacter) return;

	AISPlayerState* SourcePlayerState = IISPlayerInterface::Execute_GetPlayerState(TargetActor);
	if(!SourcePlayerState) return;

	UAbilitySystemComponent* SourceASC = SourcePlayerState->GetAbilitySystemComponent();
	if(!SourceASC) return;

	UISAttributeSet* SourceAS = Cast<UISAttributeSet>(SourcePlayerState->GetAttributeSet());
	if(!SourceAS) return;

	SourceASC->GetGameplayAttributeValueChangeDelegate(SourceAS->GetCoinsAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnTargetAttributeChange.Broadcast(Data.NewValue);
	});
	OnTargetAttributeChange.Broadcast(SourceAS->GetCoins());
}
void AISNonePlayerCharacter::SetAIState_Implementation(EAIState State)
{
	IISPublicInterface::SetAIState_Implementation(State);
	
	AIState = State;
}

EAIState AISNonePlayerCharacter::GetAIState_Implementation() const
{
	return AIState;
}