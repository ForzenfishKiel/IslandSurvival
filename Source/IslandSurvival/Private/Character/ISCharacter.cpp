// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISCharacter.h"
#include "ActorComponents/ISGearEquipComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISPlayerController.h"
#include "Game/ISPlayerMainHUD.h"
#include "Game/ISPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	CharacterInventory->SetIsReplicated(true);  //背包组件设定为可复制
	CharacterHotBarInventory = CreateDefaultSubobject<UISHotBarInventory>(TEXT("CharacterHotBarInventory"));
	CharacterHotBarInventory->SetIsReplicated(true);  //物品栏组件设定为可复制
	CharacterEquipment = CreateDefaultSubobject<UISEquipmentComponent>(TEXT("CharacterEquipment"));
	CharacterEquipment->SetIsReplicated(true);  //装备系统也设定为可复制
	ISInteractionComponent = CreateDefaultSubobject<UISInteractionComponent>(TEXT("ISInteractionComponent")); //玩家交互组件
	ISInteractionComponent->SetIsReplicated(true);
	ISCraftingComponent = CreateDefaultSubobject<UISCraftingComponent>(TEXT("ISCraftingComponent"));
	ISCraftingComponent->SetIsReplicated(true);
	GearEquipComponent = CreateDefaultSubobject<UISGearEquipComponent>(TEXT("GearEquipComponent"));  //创建玩家装备组件
	GearEquipComponent->SetIsReplicated(true);
	ISBuildingComponent = CreateDefaultSubobject<UISBuildingComponent>(TEXT("ISBuildingComponent"));
	ISBuildingComponent->SetIsReplicated(true);
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
	SetOwner(NewController);
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
	InitializePlayerAttribute(SourceASC,PlayerDefaultAttribute);
	InitializePlayerAttribute(SourceASC,PlayerSecondaryAttribute);
}

void AISCharacter::InitializePlayerAttribute(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> AttributeClass)
{
	check(AttributeClass);
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(AttributeClass,1.f,EffectContextHandle);
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

