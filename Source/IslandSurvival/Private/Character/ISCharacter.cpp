// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ISPublicInterface.h"
#include "ActorComponents/ISGearEquipComponent.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Components/CapsuleComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISGameInstance.h"
#include "Game/ISGameplayMode.h"
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

	ISTradingSystemComponent = CreateDefaultSubobject<UISTradingSystemComponent>(TEXT("TradingSystemComponent"));

	BGMManager = CreateDefaultSubobject<UISBGMManagerComponent>(TEXT("ISBGMManagerComponent"));

	BGMManager->SetIsReplicated(true);
}

void AISCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AISCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AISCharacter,CharacterSpeed);
	DOREPLIFETIME(AISCharacter,bIsDead);
}

void AISCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckIsFastRun();
}

void AISCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AISPlayerState* SourceAS = GetPlayerState<AISPlayerState>();
		
	AISPlayerController* SourcePC = Cast<AISPlayerController>(NewController);
	if(HasAuthority())
	{
		InitAbilityActorInfo();  //服务器调用初始化操作
	}
	SetOwner(NewController);

	//初始化控制器的游戏模式
	FInputModeGameOnly InputMode;
	SourcePC->SetInputMode(InputMode);
	
	LoadProgress(); //尝试加载角色的进度
	
	//AddCharacterActivateAbility(CharacterActivateAbilities);
	//AddCharacterPassiveAbility(CharacterPassiveAbilities);
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

AISPlayerState* AISCharacter::GetSourcePlayerState_Implementation()
{
	AISPlayerState*SourcePlayerState = GetPlayerState<AISPlayerState>();
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

void AISCharacter::PlayBGM(EBGMType Type, float FadeTime)
{
	if(BGMManager && GetLocalRole() != ROLE_AutonomousProxy)
	{
		BGMManager->PlayBGM(Type, FadeTime);
	}
}

void AISCharacter::StopBGM(EBGMType Type, float FadeTime)
{
	if(BGMManager && GetLocalRole() != ROLE_AutonomousProxy)
	{
		BGMManager->StopBGM(FadeTime);
	}
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
	//InitializePlayerAttribute(SourceASC,PlayerDefaultAttribute);
	//InitializePlayerAttribute(SourceASC,PlayerSecondaryAttribute);
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

void AISCharacter::Die()
{
	Super::Die();
	
	
	CameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);  //接触摄像机

	ClearPlayerMainHUD();  //删除玩家的主UI

	MenuWindowOpen(this);  //打开死亡菜单

	MulticastHandleDeath();  //多播死亡画面

	bIsDead = true;

	SetLifeSpan(10.f);
}

void AISCharacter::MulticastHandleDeath()
{
	Super::MulticastHandleDeath();

	GetMesh()->SetSimulatePhysics(true);  //开启模拟物理效果
	GetMesh()->SetEnableGravity(true); //开启重力效果
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);  //开启物理碰撞效果
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UISMenuUIBase* AISCharacter::GetMenuUI_Implementation() const
{
	AISPlayerController* SourcePC = Cast<AISPlayerController>(GetController());  //获取本地控制器
	if(!SourcePC) return nullptr;
	AISPlayerMainHUD* SourceMainHUD = Cast<AISPlayerMainHUD>(SourcePC->GetHUD());
	if(!SourceMainHUD) return nullptr;
	return SourceMainHUD->ISDieHUD;
}

//用户打开库存界面
void AISCharacter::TradWindowOpen_Implementation(AActor* TargetActor)
{
	MenuUIRef = IISPublicInterface::Execute_GetMenuUI(TargetActor);  //获取对方的UI
	if(!MenuUIRef) return;
	
	AISPlayerController* SourcePC = Cast<AISPlayerController>(GetController());
	if(!SourcePC) return;
	SourcePC->OnOpenInventoryEvent.AddDynamic(this,&AISCharacter::TradWindowClose);
	
	AISPlayerState* TargetState = GetPlayerState<AISPlayerState>(); //获取角色的状态
	if(!TargetState) return;

	MenuUIRef->InitializeCraftingData(IISPublicInterface::Execute_GetItemsContainer(TargetActor));  //获取对方的Container
	
	if(!MenuUIRef->IsVisible())
	{
		SourcePC->bIsOpenStorage = true;
		MenuUIRef->AddToViewport();
		SourcePC->bShowMouseCursor = true;
		SourcePC->SetInputMode(FInputModeGameAndUI());
		SourcePC->InputSubsystem->RemoveMappingContext(SourcePC->CharacterInputMapping);
		SourcePC->InputSubsystem->AddMappingContext(SourcePC->CharacterInputMenuMapping,0);
	}
	IISPublicInterface::Execute_BindWidgetController(TargetActor,this);
}

//打开用户菜单UI
void AISCharacter::MenuWindowOpen_Implementation(AActor* TargetActor)
{
	MenuUIRef = IISPublicInterface::Execute_GetMenuUI(TargetActor);  //获取对方的UI
	if(!MenuUIRef) return;
	
	AISPlayerController* SourcePC = Cast<AISPlayerController>(GetController());
	if(!SourcePC) return;
	SourcePC->OnOpenInventoryEvent.AddDynamic(this,&AISCharacter::TradWindowClose);
	
	AISPlayerState* TargetState = GetPlayerState<AISPlayerState>(); //获取角色的状态
	if(!TargetState) return;
	
	if(!MenuUIRef->IsVisible())
	{
		SourcePC->bIsOpenStorage = true;
		MenuUIRef->AddToViewport();
		SourcePC->bShowMouseCursor = true;
		SourcePC->SetInputMode(FInputModeGameAndUI());
		SourcePC->InputSubsystem->RemoveMappingContext(SourcePC->CharacterInputMapping);
		SourcePC->InputSubsystem->AddMappingContext(SourcePC->CharacterInputMenuMapping,0);
	}
}

//用户关闭界面
void AISCharacter::TradWindowClose_Implementation(APlayerController* TargetController)
{
	AISPlayerController* SourcePC = Cast<AISPlayerController>(TargetController);
	if(!SourcePC) return;
	UISInteractionComponent* InteractionComp = GetComponentByClass<UISInteractionComponent>();
	
	if(MenuUIRef->IsVisible())
	{
		SourcePC->bIsOpenStorage = false;
		MenuUIRef->RemoveFromParent();
		SourcePC->bShowMouseCursor = false;
		SourcePC->SetInputMode(FInputModeGameOnly());
		SourcePC->InputSubsystem->RemoveMappingContext(SourcePC->CharacterInputMenuMapping);
		SourcePC->InputSubsystem->AddMappingContext(SourcePC->CharacterInputMapping,0);
		SourcePC->OnOpenInventoryEvent.RemoveDynamic(this,&AISCharacter::TradWindowClose);
		MenuUIRef = nullptr;
	}
	if(InteractionComp)
	{
		InteractionComp->ClearInteractedActor();  //清空当前交互对象
	}
}

void AISCharacter::ClearPlayerMainHUD_Implementation()
{
	AISPlayerController* SourcePC = Cast<AISPlayerController>(GetController());  //获取本地控制器
	if(!SourcePC) return;
	AISPlayerMainHUD* SourceMainHUD = Cast<AISPlayerMainHUD>(SourcePC->GetHUD());
	if(!SourceMainHUD) return;

	SourceMainHUD->ClearMainUI(); //清除HUD的主UI
}

//保存玩家进度
void AISCharacter::SaveProgress_Implementation()
{
	IISPlayerInterface::SaveProgress_Implementation();

	if(const AISGameplayMode* GamePlayMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(this)))
	{
		//获取当前存档
		UISLocalPlayerSaveGame* SaveGameData = GamePlayMode->RetrieveInGameSaveData();
		if(SaveGameData == nullptr) return;

		SaveGameData->PlayerTransform = GetTransform();  //获取并保存当前玩家自身的位置
		
		const AISPlayerState* ISPlayerState = GetPlayerState<AISPlayerState>();

		if(ISPlayerState)
		{
			SaveGameData->AttributePoints = ISPlayerState->GetAttributePoint();  //保存玩家的属性点
			SaveGameData->PlayerLevel = ISPlayerState->GetPlayerLevel(); //保存玩家的等级
			SaveGameData->XP = ISPlayerState->GetXP();  //保存玩家的经验值
		}

		//保存玩家生命值
		SaveGameData->Health = UISAttributeSet::GetHealthAttribute().GetNumericValue(ISPlayerState->GetAttributeSet());
		//保存玩家饥饿值
		SaveGameData->Hungry = UISAttributeSet::GetHungerAttribute().GetNumericValue(ISPlayerState->GetAttributeSet());
		//保存玩家口渴值
		SaveGameData->Thirsty = UISAttributeSet:: GetThirstAttribute().GetNumericValue(ISPlayerState->GetAttributeSet());
		//保存玩家体力值
		SaveGameData->Vigor = UISAttributeSet::GetVigorAttribute().GetNumericValue(ISPlayerState->GetAttributeSet());



		UISAbilitySystemComponent* ISSourceASC = Cast<UISAbilitySystemComponent>(SourceASC);
		SaveGameData->SaveAbilities.Empty();  //清空当前存档的数组
		//使用ASC里创建的ForEach函数循环获取角色的技能，并生成技能结构体保存
		FForEachAbility SaveAbilityDelegate;
		SaveAbilityDelegate.BindLambda([this, ISSourceASC, SaveGameData](const FGameplayAbilitySpec& AbilitySpec)
		{
			//获取技能标签和
			const FGameplayTag AbilityTag = UISAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec);

			//创建技能结构体
			FSaveAbility SavedAbility;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilityTag = AbilityTag;

			SaveGameData->SaveAbilities.AddUnique(SavedAbility);  //查找是否相同再添加
		});

		const UISCharacterInventory* CharacterBackPack = GetComponentByClass<UISCharacterInventory>(); //获取玩家背包
		const UISHotBarInventory* CharacterHotBar = GetComponentByClass<UISHotBarInventory>(); //获取玩家物品栏

		
		SaveGameData->PlayerBackPack.Empty(); //清空保存的背包数据
		SaveGameData->HotBarItems.Empty(); //清空保存的物品栏数据
		
		SaveGameData->PlayerBackPack = CharacterBackPack->InventoryContainer;  //存储玩家的背包
		SaveGameData->HotBarItems = CharacterHotBar->InventoryContainer; //存储玩家的物品栏数据
		
		SaveGameData->PlayerSaveRespawnLocation = GetPlayerRespawnLocation();  //保存玩家的重生位置

		GamePlayMode->SaveInGameProgressData(SaveGameData);  //保存玩家进度
		GamePlayMode->SaveWorldState(GetWorld());  //保存世界状态

		
	}
}

//加载玩家进度
void AISCharacter::LoadProgress()
{
	UISGameInstance* ISGameInstance = Cast<UISGameInstance>(GetGameInstance());

	AISGameplayMode* ISGameplayMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(this));

	AISPlayerState* SourceAS = Cast<AISPlayerState>(GetPlayerState<AISPlayerState>());

	if(!ISGameInstance && !ISGameplayMode) return;

	//获取当前游玩的存档
	UISLocalPlayerSaveGame* LocalPlayerSaveGame = ISGameplayMode->RetrieveInGameSaveData();
	if(ISGameInstance->bIsMultPlayerMode)
	{
		SourceAS->SetLevel(1);  //设置玩家保存的等级
		SourceAS->SetXP(0);  //设置玩家保存的经验
		SourceAS->SetAttributePoint(0); //设置玩家保存的属性点
		//初始化玩家属性
		InitializePlayerAttribute(SourceASC,PlayerDefaultAttribute);
		InitializePlayerAttribute(SourceASC,PlayerSecondaryAttribute);
		
		//初始化玩家技能和被动技能
		AddCharacterActivateAbility(CharacterActivateAbilities);
		AddCharacterPassiveAbility(CharacterPassiveAbilities);
		
		AActor* PlayerDefaultRespawnLocation = ISGameplayMode->ChoosePlayerStart(GetInstigatorController()); //获取玩家默认的重生点
		IISPlayerInterface::Execute_SetPlayerRespawnLocation(this,PlayerDefaultRespawnLocation->GetActorLocation());  //设置玩家的默认重生点
		return;
	}
	
	if(! LocalPlayerSaveGame) return;
	
	SetPlayerName(LocalPlayerSaveGame->PlayerName);
	
	//玩家是否是第一次进入游戏
	if(ISGameInstance->bFirstTimeStartGame)
	{
		SourceAS->SetLevel(LocalPlayerSaveGame->PlayerLevel);  //设置玩家保存的等级
		SourceAS->SetXP(LocalPlayerSaveGame->XP);  //设置玩家保存的经验
		SourceAS->SetAttributePoint(LocalPlayerSaveGame->AttributePoints); //设置玩家保存的属性点
		//初始化玩家属性
		InitializePlayerAttribute(SourceASC,PlayerDefaultAttribute);
		InitializePlayerAttribute(SourceASC,PlayerSecondaryAttribute);
		
		//初始化玩家技能和被动技能
		AddCharacterActivateAbility(CharacterActivateAbilities);
		AddCharacterPassiveAbility(CharacterPassiveAbilities);
		
		AActor* PlayerDefaultRespawnLocation = ISGameplayMode->ChoosePlayerStart(GetInstigatorController()); //获取玩家默认的重生点
		IISPlayerInterface::Execute_SetPlayerRespawnLocation(this,PlayerDefaultRespawnLocation->GetActorLocation());  //设置玩家的默认重生点

		ISGameplayMode->SaveInGameProgressData(LocalPlayerSaveGame); //玩家第一次进入游戏的时候，服务器会保存一次游戏的进度
	}
	else
	{
		//转为加载玩家保存的属性
		UISAbilitysystemLibary::InitializeDefaultAttributesFromSaveData(this,SourceASC,LocalPlayerSaveGame);

		//继续加载玩家的技能
		AddCharacterActivateAbility(CharacterActivateAbilities);
		AddCharacterPassiveAbility(CharacterPassiveAbilities);

		UISCharacterInventory* PlayerBackPack = GetComponentByClass<UISCharacterInventory>(); //获取玩家的背包
		UISHotBarInventory* HotBarItems = GetComponentByClass<UISHotBarInventory>(); //获取玩家物品栏

		PlayerBackPack->InventoryContainer = LocalPlayerSaveGame->PlayerBackPack; //导入玩家的背包
		HotBarItems->InventoryContainer = LocalPlayerSaveGame->HotBarItems;  //导入玩家的物品栏
		PlayerBackPack->InventoryUpdate.Broadcast();
		HotBarItems->InventoryUpdate.Broadcast();

		IISPlayerInterface::Execute_SetPlayerRespawnLocation(this,LocalPlayerSaveGame->PlayerSaveRespawnLocation);  //读取玩家设置的重生点


		ISGameplayMode->LoadWorldState(GetWorld());  //加载世界状态
		SetActorTransform(LocalPlayerSaveGame->PlayerTransform);  //将玩家传送到当前保存的位置
	}
}

TSubclassOf<UGameplayEffect> AISCharacter::GetSecondaryAttributes_Implementation()
{
	return PlayerSecondaryAttribute;
}

TSubclassOf<UGameplayEffect> AISCharacter::GetPrimaryAttributes_Implementation()
{
	return PlayerDefaultAttribute;
}


//设置玩家的姓名
void AISCharacter::SetPlayerName(const FString& InPlayerName)
{
	AISPlayerState* SourceAS = GetPlayerState<AISPlayerState>();
	SourceAS->SetSourcePlayerName(InPlayerName);
}

FString AISCharacter::GetPlayerName_Implementation() const
{
	AISPlayerState* SourceAS = GetPlayerState<AISPlayerState>();
	return SourceAS->GetSourcePlayerName();
}

void AISCharacter::SetPlayerRespawnLocation_Implementation(const FVector& InLocation)
{
	IISPlayerInterface::SetPlayerRespawnLocation_Implementation(InLocation);
	AISPlayerState* SourcePS = GetPlayerState<AISPlayerState>(); //获取角色的状态
	
	SourcePS->SetPlayerRespawnLocation(InLocation);  //设置角色的重生点
}

FVector AISCharacter::GetPlayerRespawnLocation() const
{
	AISPlayerState* SourceAS = GetPlayerState<AISPlayerState>();
	return SourceAS->GetPlayerRespawnLocation();
}

