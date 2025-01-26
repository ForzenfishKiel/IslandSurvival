// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ActorComponents/ISGearEquipComponent.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Components/CapsuleComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISGameplayMode.h"
#include "Game/ISGameplayTagsManager.h"
#include "Game/ISPlayerController.h"
#include "Game/ISPlayerMainHUD.h"
#include "Game/ISPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/ISEnemyInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AISCharacter::AISCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//使用控制器去控制角色
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0, 0, 0);  //设定旋转速率

	GetCharacterMovement()->JumpZVelocity = 700.f;  //设置跳跃高度
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;  //设置移动速度
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);  //胶囊体忽视摄像机

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArmComp"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = true;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmorMesh"));
	ArmMesh->SetupAttachment(CameraComponent);
	ArmMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CharacterInventory = CreateDefaultSubobject<UISCharacterInventory>(TEXT("CharacterInventory"));  //玩家背包组件

	CharacterHotBarInventory = CreateDefaultSubobject<UISHotBarInventory>(TEXT("CharacterHotBarInventory"));

	CharacterEquipment = CreateDefaultSubobject<UISEquipmentComponent>(TEXT("CharacterEquipment"));

	ISInteractionComponent = CreateDefaultSubobject<UISInteractionComponent>(TEXT("ISInteractionComponent")); //玩家交互组件

	ISCraftingComponent = CreateDefaultSubobject<UISCraftingComponent>(TEXT("ISCraftingComponent"));

	GearEquipComponent = CreateDefaultSubobject<UISGearEquipComponent>(TEXT("GearEquipComponent"));  //创建玩家装备组件

	ISBuildingComponent = CreateDefaultSubobject<UISBuildingComponent>(TEXT("ISBuildingComponent"));

}

void AISCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AISCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AISCharacter,CharacterSpeed);
}

void AISCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckIsFastRun();
}

void AISCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(HasAuthority())
	{
		InitAbilityActorInfo();  //服务器调用初始化操作
	}
	AddCharacterActivateAbility(CharacterActivateAbilities);
	AddCharacterPassiveAbility(CharacterPassiveAbilities);
}

void AISCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();  //客户端也调用同样的初始化
}

void AISCharacter::AddToXP_Implementation(int32 XP)
{
	IISPlayerInterface::AddToXP_Implementation(XP);
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	check(ISPlayerState);
	ISPlayerState->AddToXP(XP);
}

int32 AISCharacter::FindLevelFromXP_Implementation(int32 InXP)
{
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	check(ISPlayerState);
	return ISPlayerState->ISLevelUpInformation->FindLevelFromXP(InXP);
}

int32 AISCharacter::GetXP_Implementation()
{
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	check(ISPlayerState);
	return ISPlayerState->GetXP();  //获取当前的XP
}

int32 AISCharacter::GetLevel_Implementation()
{
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	check(ISPlayerState);
	return ISPlayerState->GetPlayerLevel();
}

int32 AISCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	check(ISPlayerState);
	return ISPlayerState->ISLevelUpInformation->LevelUpArray[Level].AttributePointAward;  //从对应等级中获取当前属性点
}

int32 AISCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	check(ISPlayerState);
	return ISPlayerState->ISLevelUpInformation->LevelUpArray[Level].SpellPointAward;
}


void AISCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	check(ISPlayerState);
	ISPlayerState->AddToAttributePoint(InAttributePoints);
}

void AISCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	check(ISPlayerState);
	ISPlayerState->AddToLevel(InPlayerLevel);  //添加角色等级
}

void AISCharacter::LevelUp_Implementation()
{
	IISPlayerInterface::LevelUp_Implementation();
}

AISPlayerState* AISCharacter::GetPlayerState_Implementation()
{
	AISPlayerState*SourcePlayerState = GetPlayerState<AISPlayerState>();
	check(SourcePlayerState);
	return SourcePlayerState;
}

bool AISCharacter::CheckIsFastRun()
{
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	if (!ISPlayerState) return false;
	UISAttributeSet*ISAttributeSet = Cast<UISAttributeSet>( ISPlayerState->GetAttributeSet());
	if(ISAttributeSet->GetVigor()<=0)
	{
		GetCharacterMovement()->MaxWalkSpeed = CharacterSpeed;
		return false;
	}
	//如果角色拥有在地面上的速度且此时处于短跑阶段，则返回角色处于短跑
	if(GetCharacterMovement()->Velocity.Size()>0&&GetCharacterMovement()->GetMaxSpeed()>400)
	{
		return true;
	}
	//角色速度未达到短跑速度则恢复为走路速度
	else if(GetCharacterMovement()->Velocity.Size()<600)
	{
		GetCharacterMovement()->MaxWalkSpeed = CharacterSpeed;
		return false;
	}
	return false;
}

void AISCharacter::InitAbilityActorInfo()
{
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	check(ISPlayerState); //找不到角色游戏状态则停止
	ISPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ISPlayerState,this);  //初始化设定角色ASC
	SourceASC = ISPlayerState->GetAbilitySystemComponent();
	UAttributeSet*SourceAS = ISPlayerState->GetAttributeSet();
	if(AISPlayerController*ISPlayerController = Cast<AISPlayerController>(GetController()))
	{
		FCharacterParams CharacterParams(SourceASC,SourceAS,ISPlayerController,ISPlayerState);  //初始化好角色的属性数据
		AISPlayerMainHUD*PlayerHUD = Cast<AISPlayerMainHUD>(ISPlayerController->GetHUD());  //获取HUD
		if(PlayerHUD)
		{
			PlayerHUD->InitUserWidget(CharacterParams);
		}
	}
	CharacterEquipment->InitializeEquipmentComponent(SourceASC);
	BindAttributeSet();  //绑定一些部分的值
	InitializePlayerAttribute(SourceASC,PlayerDefaultAttribute);
	InitializePlayerAttribute(SourceASC,PlayerSecondaryAttribute);
}

void AISCharacter::InitializePlayerAttribute(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> AttributeClass)
{
	check(AttributeClass);
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(AttributeClass,GetLevel_Implementation(),EffectContextHandle);
	SourceASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void AISCharacter::AddCharacterActivateAbility(TArray<TSubclassOf<UGameplayAbility>>& TargetActivateAbilities)
{
	UISAbilitySystemComponent*LocalASC = Cast<UISAbilitySystemComponent>(SourceASC);
	if(!LocalASC)return;
	LocalASC->AddCharacterAbility(TargetActivateAbilities);  //添加角色可激活执行的技能
}

void AISCharacter::AddCharacterPassiveAbility(TArray<TSubclassOf<UGameplayAbility>>& TargetActivateAbilities)
{
	UISAbilitySystemComponent*LocalASC = Cast<UISAbilitySystemComponent>(SourceASC);
	if(!LocalASC) return;
	LocalASC->AddCharacterPassiveAbility(TargetActivateAbilities); //应用并激活被动技能
}

void AISCharacter::AddAttributeLevel_Implementation(const FGameplayAttribute TargetPointType)
{
	AISPlayerState*ISPlayerState = GetPlayerState<AISPlayerState>();
	ISPlayerState->AddTargetAttributeLevel(TargetPointType);
}

void AISCharacter::SetOwnerWhenCharacterControlActor_Implementation(const TArray<UISItemsContainer*>&InItemsContainer,
	APlayerController* InController)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("玩家控制器调用"));

	for(auto ContainerRef : InItemsContainer)
	{
		AActor* ContainerOwner = ContainerRef->GetOwner();
		if(ContainerOwner->GetOwner() == nullptr)
		{
			ContainerOwner->SetOwner(InController);
		}
		else
		{
			if(ContainerOwner->GetOwner() != InController)
			{
				ContainerOwner->SetOwner(InController);
			}
		}
	}
}


void AISCharacter::SetSingleOwnerWhenCharacterControlActor_Implementation(const UISItemsContainer* InItemsContainer,
	APlayerController* InController)
{
	AActor* TargetActor = InItemsContainer->GetOwner();
	if(!TargetActor) return;
	if(TargetActor->GetOwner() == nullptr)
	{
		TargetActor->SetOwner(InController);
	}
	else
	{
		if(TargetActor->GetOwner() != InController)
		{
			TargetActor->SetOwner(InController);
		}
	}
}
TSubclassOf<AISHarvestingBase> AISCharacter::CheckHarvestDataAsset(const FString& InName)
{
	AISGameplayMode* SourceGamemomde = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(GetOwner()));
	if(!SourceGamemomde)return nullptr;

	UISHarvestDataAsset* HarvestDataAsset = UISAbilitysystemLibary::GetHarvestDataAsset(GetOwner());
	if(!HarvestDataAsset)return nullptr;

	return HarvestDataAsset->GetHarvest(InName); //返回对应的类模板
}

AISHarvestingBase* AISCharacter::ReplaceToActor(const TSubclassOf<AISHarvestingBase> InClassTemp,
	UInstancedStaticMeshComponent* TargetComponent, const int32 TargetItemNum)
{
	FTransform OutTransform;
	if(!TargetComponent->GetInstanceTransform(TargetItemNum,OutTransform,true)) return nullptr;
	MulticastToRemoveStaticMesh(TargetComponent,TargetItemNum); //删除原来的模型
	
	AISHarvestingBase* HarvestingBaseRef = GetWorld()->SpawnActor<AISHarvestingBase>(InClassTemp,OutTransform); //在原位置生成Actor
	return HarvestingBaseRef;
}

void AISCharacter::MulticastToRemoveStaticMesh_Implementation(UInstancedStaticMeshComponent* TargetComponent,
	const int32 TargetItemNum)
{
	if(!IsValid(TargetComponent) && TargetItemNum <= 0) return;
	TargetComponent->RemoveInstance(TargetItemNum);
}

void AISCharacter::BindAttributeSet() const
{
	AISPlayerState* SourcePS = GetPlayerState<AISPlayerState>();
	UAbilitySystemComponent* Asc = Cast<UAbilitySystemComponent>(SourcePS->GetAbilitySystemComponent());
	UISAttributeSet* SourceAS = Cast<UISAttributeSet>(SourcePS->GetAttributeSet());

	if(!Asc) return;

	Asc->GetGameplayAttributeValueChangeDelegate(SourceAS->GetMaxSpeedAttribute()).AddLambda([this]
		(const FOnAttributeChangeData& Data)
	{
		GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
	}
		);
}

void AISCharacter::ApplyDamageToTarget_Implementation(AActor* Target)
{
	if(!Target) return;
	if(!Target->Implements<UISEnemyInterface>()) return;
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if(!TargetASC) return;
	AISPlayerState* SourcePS =  GetPlayerState<AISPlayerState>();
	UAbilitySystemComponent* SourcePlayerASC = SourcePS->GetAbilitySystemComponent();
	if(!SourcePlayerASC) return;

	int32 CharacterLevel = SourcePS->GetPlayerLevel();

	check(CharacterTakeDamageEffectClass);

	FGameplayEffectContextHandle GameplayEffectContextHandle = SourcePlayerASC->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = SourcePlayerASC->MakeOutgoingSpec(CharacterTakeDamageEffectClass,CharacterLevel,GameplayEffectContextHandle);
	UISAttributeSet* SourceAS = Cast<UISAttributeSet>(SourcePS->GetAttributeSet());
	if(!SourceAS) return;

	
	const FActiveGameplayEffectHandle ActivateGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
}

AISCharacter* AISCharacter::GetSourceCharacter_Implementation()
{
	return this;
}
