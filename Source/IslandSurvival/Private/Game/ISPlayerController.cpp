// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActorComponents/ISHotBarInventory.h"
#include "Character/ISCharacter.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISEnhancedInputComponent.h"
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
		,&AISPlayerController::InputHeldAbility,&AISPlayerController::InputHeldAbility);
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
	
}

void AISPlayerController::InputReleasedAbility(const FGameplayTag InputTag)
{
	
}

AISCharacter* AISPlayerController::GetCharacterLocal() const
{
	return Cast<AISCharacter>(GetPawn());
}
