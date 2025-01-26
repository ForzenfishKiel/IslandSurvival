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
	NPCAsc->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); //最小地去复制npc的一些属性
	NPCTradComponent = CreateDefaultSubobject<UISTradingSystemComponent>(TEXT("TradingSystemComponent"));  //创建可交易允许的组件
}

AISNonePlayerCharacter* AISNonePlayerCharacter::GetNPC_Implementation()
{
	return this;
}

void AISNonePlayerCharacter::OnNPCWasInteracted_Implementation(AActor* InteractingActor)
{
	IISNPCInterface::OnNPCWasInteracted_Implementation(InteractingActor);
	APlayerController* SourcePC = Cast<APlayerController>( InteractingActor->GetInstigatorController());
	if(SourcePC)
	{
		TradWindowOpen(SourcePC);  //客户端打开UI
	}
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
}

//调用NPC的获取其ASC的方法
UAbilitySystemComponent* AISNonePlayerCharacter::GetAbilitySystemComponent() const
{
	return NPCAsc;
}

void AISNonePlayerCharacter::TradWindowOpen_Implementation(APlayerController* TargetController)
{
	check(TraderUserWidget);

	AISPlayerController* SourcePC = Cast<AISPlayerController>(TargetController);  //转换成本地PC
	if(!SourcePC) return;
	SourcePC->OnOpenInventoryEvent.AddDynamic(this,&AISNonePlayerCharacter::TradWindowClose);

	AISCharacter* TargetCharacter = IISPlayerInterface::Execute_GetSourceCharacter(SourcePC->GetPawn());
	if(!TargetCharacter) return;
	AISPlayerState* TargetState = TargetCharacter->GetPlayerState<AISPlayerState>(); //获取角色的状态
	if(!TargetState) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetState);
	if(!TargetASC) return;
	UISAttributeSet* TargetAS = Cast<UISAttributeSet>( TargetState->GetAttributeSet() );
	TargetASC->GetGameplayAttributeValueChangeDelegate(TargetAS->GetCoinsAttribute()).AddLambda([this]
		(const FOnAttributeChangeData& Data)
	{
		OnTargetAttributeChange.Broadcast(Data.NewValue);  //广播角色的金币
	}
	);
	
	
	if(!TraderUserWidget->IsVisible())
	{
		SourcePC->bIsOpenStorage = true;
		TraderUserWidget->AddToViewport();
		SourcePC->bShowMouseCursor = true;
		SourcePC->SetInputMode(FInputModeGameAndUI());
		SourcePC->InputSubsystem->RemoveMappingContext(SourcePC->CharacterInputMapping);
		SourcePC->InputSubsystem->AddMappingContext(SourcePC->CharacterInputMenuMapping,0);
	}
}

void AISNonePlayerCharacter::TradWindowClose_Implementation(APlayerController* TargetController)
{
	AISPlayerController* SourcePC = Cast<AISPlayerController>(TargetController);
	if(!SourcePC) return;

	
	if(TraderUserWidget->IsVisible())
	{
		SourcePC->bIsOpenStorage = false;
		TraderUserWidget->RemoveFromParent();
		SourcePC->bShowMouseCursor = false;
		SourcePC->SetInputMode(FInputModeGameOnly());
		SourcePC->InputSubsystem->RemoveMappingContext(SourcePC->CharacterInputMenuMapping);
		SourcePC->InputSubsystem->AddMappingContext(SourcePC->CharacterInputMapping,0);
		SourcePC->OnOpenInventoryEvent.RemoveDynamic(this,&AISNonePlayerCharacter::TradWindowClose);
	}
}
