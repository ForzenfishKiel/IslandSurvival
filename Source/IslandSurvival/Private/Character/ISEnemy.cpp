// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISEnemy.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Components/CapsuleComponent.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISAttributeSet.h"
#include "Game/ISGameplayTagsManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Interface/ISPlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/ISMainUIBase.h"

AISEnemy::AISEnemy()
{
	NetUpdateFrequency = 100.f;  //网络更新速率
	MinNetUpdateFrequency = 66.f;

	ISEnemyAbilitysystem = CreateDefaultSubobject<UISAbilitySystemComponent>(TEXT("ISEnemyAbilityComponent"));
	ISEnemyAbilitysystem->SetIsReplicated(true);
	ISEnemyAbilitysystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);  //仅复制最少量信息
	ISEnemyAttribute = CreateDefaultSubobject<UISAttributeSet>(TEXT("ISEnemyAttribute"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetIsReplicated(true);
}

void AISEnemy::SetAISpeed(const EAISpeed InState)
{
	GetCharacterMovement()->MaxWalkSpeed = *AISpeedManager.Find(InState);
}

void AISEnemy::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ISEnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDie"), true);  //改变行为树的值
	
	const FGameplayTagsManager& GameplayTags = FGameplayTagsManager::Get();
	FGameplayEventData Payload; //创建Payload
	Payload.EventTag = GameplayTags.AI_State_Dying;
	Payload.EventMagnitude = 0.f;  //输送经验和对应的Tag
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this,  GameplayTags.AI_State_Dying,Payload);  //向其GA发送死亡事件

}

void AISEnemy::MulticastHandleDeath()
{
	Super::MulticastHandleDeath();
	
}

UAbilitySystemComponent* AISEnemy::GetAbilitySystemComponent() const
{
	return ISEnemyAbilitysystem;
}

void AISEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(!HasAuthority()) return;  //确保只在服务器运行

	ISEnemyAIController = Cast<AISAIController>(NewController);

	ISEnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

	ISEnemyAIController->RunBehaviorTree(BehaviorTree);  //运行行为树
	
}

void AISEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();  //初始化角色技能信息

	if(HasAuthority())
	{
		UISAbilitysystemLibary::GiveStartupAbilities(this,ISEnemyAbilitysystem,CharacterName);
	}

	if(UISMainUIBase* HealthWidget = Cast<UISMainUIBase>(HealthBar->GetUserWidgetObject()))
	{
		HealthWidget->SetWidgetController(this);
	}

	if(const UISAttributeSet* AS = Cast<UISAttributeSet>(GetEnemyAttribute()))
	{
		ISEnemyAbilitysystem->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda([this]
				(const FOnAttributeChangeData& Data)
			{
			
				const UISAttributeSet* AS = Cast<UISAttributeSet>(GetEnemyAttribute());
				Health = AS->GetHealth();
				OnHealthChange.Broadcast(Health,Data.NewValue);
			}
		);	
		ISEnemyAbilitysystem->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda([this]
				(const FOnAttributeChangeData& Data)
			{

				const UISAttributeSet* AS = Cast<UISAttributeSet>(GetEnemyAttribute());
				MaxHealth = AS->GetMaxHealth();
				OnHealthChange.Broadcast(Data.NewValue,MaxHealth);
			}
		);
		OnHealthChange.Broadcast(Health,MaxHealth);
	}
}

UAttributeSet* AISEnemy::GetEnemyAttribute() const
{
	return ISEnemyAttribute;
}

void AISEnemy::InitAbilityActorInfo()
{
	ISEnemyAbilitysystem->InitAbilityActorInfo(this, this);

	if(HasAuthority())
	{
		InitializePlayerAttribute(ISEnemyAbilitysystem,nullptr);
	}
}
//初始化角色属性
void AISEnemy::InitializePlayerAttribute(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> AttributeClass)
{
	UISAbilitysystemLibary::InitializeCharacterAttributes(this,CharacterName,Level,ASC);
}

void AISEnemy::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AISEnemy,AIState);
	DOREPLIFETIME(AISEnemy,Health);
	DOREPLIFETIME(AISEnemy,MaxHealth);
}

int32 AISEnemy::GetLevel_Implementation()
{
	return Level;
}


AISAIController* AISEnemy::GetAIController_Implementation()
{
	return Cast<AISAIController>(GetController());
}

EAIState AISEnemy::GetAIState_Implementation() const
{
	return AIState;
}

void AISEnemy::SetAIState_Implementation(EAIState State)
{
	AIState = State;
}

AISEnemy* AISEnemy::GetEnemy_Implementation()
{
	return this;
}


FGameplayAbilitySpecHandle AISEnemy::FindActivateAbility_Implementation(const FGameplayTag InTag) const
{
	for( const FGameplayAbilitySpec& AbilitySpec : ISEnemyAbilitysystem->GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.HasTagExact(InTag) && !AbilitySpec.IsActive())
		{
			return AbilitySpec.Handle;
		}
	}
	return FGameplayAbilitySpecHandle();
}

void AISEnemy::ApplyDamageToTarget_Implementation(AActor* Target)
{
	AISPlayerState* CharacterState = IISPlayerInterface::Execute_GetPlayerState(Target);
	if(!CharacterState) return;

	UAbilitySystemComponent* TargetASC = CharacterState->GetAbilitySystemComponent();
	if(!TargetASC) return;

	FGameplayEffectContextHandle EffectContextHandle = ISEnemyAbilitysystem->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = ISEnemyAbilitysystem->MakeOutgoingSpec(CharacterTakeDamageEffectClass,Level,EffectContextHandle);
	UISAttributeSet* SourceAS = Cast<UISAttributeSet> (ISEnemyAttribute);
	if(!SourceAS) return;
	
	const FActiveGameplayEffectHandle ActivateEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
}

//生成敌人的尸体，用于可采集
void AISEnemy::SpawnEnemyCorpse_Implementation()
{

	IISEnemyInterface::SpawnEnemyCorpse_Implementation();

	AISHarvestingBase* EnemyCorpse = GetWorld()->SpawnActorDeferred<AISHarvestingBase>(Corpse,GetActorTransform());  //在原地生成一个动物尸体

	EnemyCorpse->CollectibleMaxHP = 100.f;
	EnemyCorpse->CollectibleHP = EnemyCorpse->CollectibleMaxHP;
	EnemyCorpse->CollectibleClass = ECollectibleClass::Corpse;
	EnemyCorpse->HarvestStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyCorpse->CollectibleName = CharacterName; 
	
	UGameplayStatics::FinishSpawningActor(EnemyCorpse,GetActorTransform());
	
	Destroy();
	
}

void AISEnemy::OnRep_MaxHealth()
{
	OnHealthChange.Broadcast(Health,MaxHealth);
}

void AISEnemy::OnRep_Health()
{
	OnHealthChange.Broadcast(Health,MaxHealth);
}
