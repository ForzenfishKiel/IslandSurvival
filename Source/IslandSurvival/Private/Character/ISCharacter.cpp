// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISGameInstance.h"
#include "Game/ISPlayerController.h"
#include "Game/ISPlayerMainHUD.h"
#include "Game/ISPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
}

void AISCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();  //服务器调用初始化操作
	SetOwner(NewController);
	AddCharacterActivateAbility(CharacterActivateAbilities);
}

void AISCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();  //客户端也调用同样的初始化
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
	check(ASC&&AttributeClass);
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(AttributeClass,1,EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(),ASC);
}

void AISCharacter::AddCharacterActivateAbility(TArray<TSubclassOf<UGameplayAbility>>& TargetActivateAbilities)
{
	UISAbilitySystemComponent*LocalASC = Cast<UISAbilitySystemComponent>(SourceASC);
	if(!LocalASC)return;
	LocalASC->AddCharacterAbility(TargetActivateAbilities);  //添加角色可激活执行的技能
}

int32 AISCharacter::FindCheckCharacterBackPack(const UDataTable*TargetDT,const int32 TargetID,const int32 RequireID)
{
	//本地运行
	UISGameInstance*ISGameplayInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));  //获取本地游戏实例
	if(!ISGameplayInstance) return 0;
	UISCraftingContainer*CraftingContainer = ISGameplayInstance->ItemContainer;
	if(!CraftingContainer) return 0;
	if(!CharacterHotBarInventory&&CraftingContainer) return 0;
	int32 Result = 0;
	FName Trans = FName(FString::Printf(TEXT("%d"),TargetID));
	
	if(FItemRecipe*UserInfo = TargetDT->FindRow<FItemRecipe>(Trans,TEXT("name")))
	{
		for(auto&DataTableRef:UserInfo->ItemRequired)
		{
			for(int32 Index = 0;Index<CharacterHotBarInventory->InventoryContainer.Num();Index++)
			{
				if(CharacterHotBarInventory->InventoryContainer[Index].ItemID==RequireID)
				{
					const int32 Value = CharacterHotBarInventory->InventoryContainer[Index].ItemQuantity;
					Result += Value;
				}
			}
			for(int32 Index = 0;Index<CharacterInventory->InventoryContainer.Num();Index++)
			{
				if(CharacterInventory->InventoryContainer[Index].ItemID==RequireID)
				{
					const int32 Value = CharacterInventory->InventoryContainer[Index].ItemQuantity;
					Result += Value;
				}
			}
			return Result;
		}
	}
	return 0;
}