// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActorComponents/ISHotBarInventory.h"
#include "Character/ISCharacter.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISEnhancedInputComponent.h"
#include "Game/ISGameplayMode.h"
#include "Game/ISPlayerMainHUD.h"
#include "Game/ISPlayerState.h"

AISPlayerController::AISPlayerController()
{
	bReplicates = true;  //允许将数据传送到服务器更新，服务器查找玩家时通过控制器进行查找
}
void AISPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(CharacterInputMapping);
	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(InputSubsystem)
	{
		InputSubsystem->AddMappingContext(CharacterInputMapping,0);  //添加增强输入到子系统
	}
	//初始化控制器的游戏模式
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AISPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UISEnhancedInputComponent*ISEnhanceInputComponent = CastChecked<UISEnhancedInputComponent>(InputComponent);
	check(ISEnhanceInputComponent);
	ISEnhanceInputComponent->BindAction(IA_Move,ETriggerEvent::Triggered,this,&AISPlayerController::Move);  //绑定移动事件
	ISEnhanceInputComponent->BindAction(IA_LookUp,ETriggerEvent::Triggered,this,&AISPlayerController::LookUp);
	ISEnhanceInputComponent->BindAction(IA_OpenUI,ETriggerEvent::Started,this,&AISPlayerController::OpenUI);
	ISEnhanceInputComponent->BindAction(IA_LeftClick,ETriggerEvent::Started,this,&AISPlayerController::PrimaryInteract);
	ISEnhanceInputComponent->BindChooseItemActions(ChooseHotBarInputData,this,&AISPlayerController::ChooseHotBar);
	ISEnhanceInputComponent->BindAbilityActions(InputAbilityData,this,&AISPlayerController::InputPressedAbility
		,&AISPlayerController::InputHeldAbility,&AISPlayerController::InputReleasedAbility);
	ISEnhanceInputComponent->BindAction(IA_SecInteract,ETriggerEvent::Started,this,&AISPlayerController::SecondaryInteract);
	ISEnhanceInputComponent->BindAction(IA_DemoBuilding,ETriggerEvent::Triggered,this,&AISPlayerController::OneClickToDemoBuilding);
	IA_PauseGame->bTriggerWhenPaused = true;
	ISEnhanceInputComponent->BindAction(IA_PauseGame,ETriggerEvent::Started,this,&AISPlayerController::PauseGame);
}

void AISPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	/*获取角色向量在平面方向的坐标*/
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection =FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Input X: %f, Y: %f"), InputAxisVector.X, InputAxisVector.Y));

	
	if(APawn*MyPawn = GetPawn<APawn>())
	{
		/*添加移动输入操作*/
		MyPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		MyPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}

void AISPlayerController::LookUp(const struct FInputActionValue& InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();  //获取数据传入的2D向量
	AISCharacter*OwingCharacter = Cast<AISCharacter>(GetPawn());
	check(OwingCharacter);
	OwingCharacter->AddControllerYawInput(LookAxisVector.X);
	OwingCharacter->AddControllerPitchInput(-(LookAxisVector.Y));
}

void AISPlayerController::PrimaryInteract()
{
	AISCharacter*OwingCharacter = Cast<AISCharacter>(GetPawn());
	UISBuildingComponent*BuildingComponent = OwingCharacter->GetComponentByClass<UISBuildingComponent>();
	UISHotBarInventory*ItemsContainer = OwingCharacter->GetComponentByClass<UISHotBarInventory>();
	if(!BuildingComponent) return;
	if(BuildingComponent->ISBuildingRef)
	{
		if(BuildingComponent->bCanBeBuild)
		{
			BuildingComponent->SpawnBuildOnServer(BuildingComponent->ISBuildingRef.GetClass(),
				BuildingComponent->ISBuildingTransformRef,BuildingComponent->bBuildPreviewWasCreated);  //通知服务器生成建筑
			ItemsContainer->ItemDiscard.Broadcast(BuildingComponent->SaveHotBarIndex,1);
		}
		else
		{
			return;
		}
	}
}

void AISPlayerController::ChooseHotBar(int32 InputIndex)
{
	if(InputIndex==-1) return;  //系统错误
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetPawn());
	UISHotBarInventory*PlayerHotBar = SourceCharacter->CharacterHotBarInventory;  //获取角色的物品栏系统
	PlayerHotBar->WhenInventoryChange(PlayerHotBar,InputIndex);
}

void AISPlayerController::OpenUI_Implementation()
{
	AISPlayerMainHUD*SourceHUD = Cast<AISPlayerMainHUD>(GetHUD()); //获取角色HUD
	UISMenuUIBase*MenuUI = SourceHUD->ISMenuUI;
	AISCharacter*SourceChar = Cast<AISCharacter>(GetPawn());
	UISItemsContainer*CharacterInventory = SourceChar->GetComponentByClass<UISItemsContainer>();

	if(bIsOpenStorage)
	{
		OnOpenInventoryEvent.Broadcast(this);
		return;
	}
	
	if(!SourceHUD&&!MenuUI) return;
	if(MenuUI->IsVisible())
	{
		MenuUI->RemoveFromParent();
		bShowMouseCursor = false;  //不显示鼠标，回归正常的游戏状态
		SetInputMode(FInputModeGameOnly());
		InputSubsystem->RemoveMappingContext(CharacterInputMenuMapping);
		InputSubsystem->AddMappingContext(CharacterInputMapping,0);
	}
	else
	{
		MenuUI->InitializeCraftingData(CharacterInventory);
		MenuUI->AddToViewport();
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
		InputSubsystem->RemoveMappingContext(CharacterInputMapping);
		InputSubsystem->AddMappingContext(CharacterInputMenuMapping,0);
	}
}

void AISPlayerController::InputPressedAbility(const FGameplayTag InputTag)
{
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetPawn());
	AISPlayerState*SourcePlayerState = SourceCharacter->GetPlayerState<AISPlayerState>();
	if(SourcePlayerState)
	{
		UISAbilitySystemComponent*LocalASC =Cast<UISAbilitySystemComponent>( UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourcePlayerState));
		if(!LocalASC) return;
		LocalASC->InputPressedFunc(InputTag);
	}
}

void AISPlayerController::InputHeldAbility(const FGameplayTag InputTag)
{
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetPawn());
	AISPlayerState*SourcePlayerState = SourceCharacter->GetPlayerState<AISPlayerState>();
	if(SourcePlayerState)
	{
		UISAbilitySystemComponent*LocalASC =Cast<UISAbilitySystemComponent>( UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourcePlayerState));
		if(!LocalASC) return;
		LocalASC->InputHoldFunc(InputTag);
	}
}

void AISPlayerController::InputReleasedAbility(const FGameplayTag InputTag)
{
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetPawn());
	AISPlayerState*SourcePlayerState = SourceCharacter->GetPlayerState<AISPlayerState>();
	if(SourcePlayerState)
	{
		UISAbilitySystemComponent*LocalASC =Cast<UISAbilitySystemComponent>( UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourcePlayerState));
		if(!LocalASC) return;
		LocalASC->InputReleased(InputTag);
	}
}

void AISPlayerController::SecondaryInteract()
{
	AISCharacter* SourceCharacter = Cast<AISCharacter>(GetPawn());
	if(!SourceCharacter) return;
	UISInteractionComponent* InteractionComponent = SourceCharacter->GetComponentByClass<UISInteractionComponent>();
	if(!InteractionComponent) return;
	InteractionComponent->SecondaryInteract();
}
//销毁当前检测的建筑
void AISPlayerController::OneClickToDemoBuilding()
{
	AISCharacter* SourceCharacter = Cast<AISCharacter>(GetPawn());
	UISBuildingComponent* BuildingComponent = SourceCharacter->GetComponentByClass<UISBuildingComponent>();
	UISInteractionComponent*InteractionComponent = SourceCharacter->GetComponentByClass<UISInteractionComponent>();
	if(!BuildingComponent) return;
	if(!InteractionComponent->LastBuildingActor) return;
	BuildingComponent->OneClickToDemoBuilding(InteractionComponent->LastBuildingActor);
}

void AISPlayerController::PauseGame()
{
	AISPlayerMainHUD* PlayerMainHUD = Cast<AISPlayerMainHUD>(GetHUD());
	PlayerMainHUD->PauseGameUIOpen();  //暂停游戏
}

//返回主菜单选项
void AISPlayerController::QuitGameEvent_Implementation()
{
	AISGameplayMode* ISGameplayMode = Cast<AISGameplayMode>(UGameplayStatics::GetGameMode(this));  //获取游戏模式
	ISGameplayMode->ReturnToMainMenu();
}

void AISPlayerController::RespawnPlayer_Implementation()
{
	check(CharacterClass);
	UnPossess();  //取消控制器的控制
	AISPlayerState* SourceAS = GetPlayerState<AISPlayerState>(); //获取角色状态

	AISCharacter* SpawnCharacter = GetWorld()->SpawnActor<AISCharacter>(CharacterClass);
	if(SpawnCharacter)
	{
		SpawnCharacter->SetActorLocation(SourceAS->GetPlayerRespawnLocation());
		Possess(SpawnCharacter);  //控制器重新控制玩家
	}
}

AISCharacter* AISPlayerController::GetCharacterLocal() const
{
	return Cast<AISCharacter>(GetPawn());
}

