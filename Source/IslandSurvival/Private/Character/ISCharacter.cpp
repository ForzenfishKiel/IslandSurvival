// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISCharacter.h"

#include "Components/CapsuleComponent.h"
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
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 0, 0);  //设定旋转速率

	GetCharacterMovement()->JumpZVelocity = 700.f;  //设置跳跃高度
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;  //设置移动速度
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);  //胶囊体忽视摄像机

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArmComp"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmorMesh"));
	ArmMesh->SetupAttachment(GetMesh());
	ArmMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CharacterInventory = CreateDefaultSubobject<UISCharacterInventory>(TEXT("CharacterInventory"));  //玩家背包组件
	CharacterInventory->SetIsReplicated(true);
	ISInteractionComponent = CreateDefaultSubobject<UISInteractionComponent>(TEXT("ISInteractionComponent")); //玩家交互组件
}

void AISCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();  //服务器调用初始化操作
}

void AISCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();  //客户端也调用同样的初始化
}

void AISCharacter::InitAbilityActorInfo()
{
	if(AISPlayerController*ISPlayerController = Cast<AISPlayerController>(GetController()))
	{
		AISPlayerState*ISPlayerState = Cast<AISPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));
		check(ISPlayerState); //找不到角色游戏状态则停止
		ISPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ISPlayerState,this);  //初始化设定角色ASC
		SourceASC = ISPlayerState->GetAbilitySystemComponent();
		UAttributeSet*SourceAS = ISPlayerState->GetAttributeSet();
		FCharacterParams CharacterParams(SourceASC,SourceAS,ISPlayerController,ISPlayerState);  //初始化好角色的属性数据
		AISPlayerMainHUD*PlayerHUD = Cast<AISPlayerMainHUD>(ISPlayerController->GetHUD());  //获取HUD
		if(PlayerHUD)
		{
			PlayerHUD->InitUserWidget(CharacterParams);
		}
	}
}