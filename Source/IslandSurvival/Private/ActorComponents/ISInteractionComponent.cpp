// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISInteractionComponent.h"
#include "Character/ISCharacter.h"
#include "Character/ISNonePlayerCharacter.h"
#include "Game/ISPlayerController.h"
#include "Game/ISPlayerMainHUD.h"
#include "Net/UnrealNetwork.h"
#include "Interface/ISItemInterface.h"
#include "Interface/ISNPCInterface.h"

// Sets default values for this component's properties
UISInteractionComponent::UISInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true); //默认打开复制
}

//Not Tick
void UISInteractionComponent::PrimaryInteract_Implementation()
{
	AISCharacter*ISCharacter = Cast<AISCharacter>(GetOwner());
	if(!ISCharacter) return;
	UCameraComponent*SourceCamera = ISCharacter->CameraComponent;
	FVector EyeLocation = SourceCamera->GetComponentLocation();
	FVector EyeForwardLocation = SourceCamera->GetForwardVector();
	FVector End = EyeLocation+(EyeForwardLocation*120);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,End,ECC_GameTraceChannel1);  //待添加一个自定义通道
	DrawDebugLine(GetWorld(), EyeLocation, Hit.GetActor() ? Hit.Location : End, FColor::Red, false, 1.0f);
	AActor* HitActor = Hit.GetActor();
	UActorComponent* HitComponent = Hit.GetComponent();
	if(HitActor||HitComponent)
	{
		AActor*CompOwner = HitComponent->GetOwner();
		if(LastComponent==HitComponent) return;
		else{LastComponent=HitComponent;}
		if(LastActor==HitActor) return;
		else{LastActor = HitActor;}
		if(CompOwner->Implements<UISBuildInterface>())
		{
			IISBuildInterface::Execute_OnBuildingWasInteract(CompOwner, GetOwner(), HitComponent);
			return;
		}
		else if(HitActor->Implements<UISBuildInterface>())
		{
			IISBuildInterface::Execute_OnBuildingWasInteract(HitActor, GetOwner(), HitComponent);
			return;
		}
		if(HitActor->Implements<UISItemInterface>())
		{
			APawn* MyPawn = Cast<APawn>(GetOwner());
			AISCharacter*SourceCharacter = Cast<AISCharacter>(GetOwner());
			UISItemsContainer*UIItems = Cast<UISItemsContainer>(SourceCharacter->GetComponentByClass<UISItemsContainer>());
			UIItems->PickUpItemForActor(SourceCharacter, HitActor);
		}
	}
}
//这个交互模式是指角色在识别到可交互的物品时，只要允许交互，无论角色当前是否处于某种状态，都会执行，例如角色打开箱子，按下按钮，捡起石头
void UISInteractionComponent::SecondaryInteract_Implementation()
{
	AISCharacter*ISCharacter = Cast<AISCharacter>(GetOwner());
	if(!ISCharacter) return;
	UCameraComponent*SourceCamera = ISCharacter->CameraComponent;
	FVector EyeLocation = SourceCamera->GetComponentLocation();
	FVector EyeForwardLocation = SourceCamera->GetForwardVector();
	FVector End = EyeLocation+(EyeForwardLocation*180);

	FHitResult Hit;
	if(GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,End,ECC_GameTraceChannel1))
	{
		DrawDebugLine(GetWorld(), EyeLocation, Hit.GetActor() ? Hit.Location : End, FColor::Green, false, 1.0f);
		AActor* HitActor = Hit.GetActor();
		UActorComponent* HitComponent = Hit.GetComponent();
		if(HitActor||HitComponent)
		{
			AActor*CompOwner = HitComponent->GetOwner();
			if(CompOwner->Implements<UISBuildInterface>())
			{
				CurrentBuilding = IISBuildInterface::Execute_GetBuildingSystemBase(CompOwner);
				AISPlayerController* SourcePC = Cast<AISPlayerController>(GetOwner()->GetInstigatorController());
				SourcePC->OnOpenInventoryEvent.AddDynamic(this,&UISInteractionComponent::ReciveControllerOpenUIEvent);
				PossessedObjectOnServer<AISBuildingSystemBase>(CurrentBuilding);
				
				IISBuildInterface::Execute_OnBuildingWasInteract(CompOwner, GetOwner(), HitComponent);
				return;
			}
			if(HitActor->Implements<UISEquipableInterface>())
			{
				PossessedObjectOnServer<AISEquipable>(IISEquipableInterface::Execute_GetEquipable(HitActor));
				IISEquipableInterface::Execute_OnEquipableWasInteract(HitActor, GetOwner());
				return;
			}
			if(HitActor->Implements<UISNPCInterface>())
			{

				PossessedObjectOnClient(HitActor);
			}
			if(HitActor->Implements<UISBuildInterface>())
			{
				CurrentBuilding = IISBuildInterface::Execute_GetBuildingSystemBase(CompOwner);
				AISPlayerController* SourcePC = Cast<AISPlayerController>(GetOwner()->GetInstigatorController());
				SourcePC->OnOpenInventoryEvent.AddDynamic(this,&UISInteractionComponent::ReciveControllerOpenUIEvent);
				PossessedObjectOnServer<AISBuildingSystemBase>(CurrentBuilding);
				
				IISBuildInterface::Execute_OnBuildingWasInteract(HitActor, GetOwner(), HitComponent);
				return;
			}
		}
	}
}


void UISInteractionComponent::PossessedObjectOnClient_Implementation(AActor* TargetActor)
{
	PossessedObjectOnServer<AISNonePlayerCharacter>(IISNPCInterface::Execute_GetNPC(TargetActor));
	IISNPCInterface::Execute_OnNPCWasInteracted(TargetActor, GetOwner());
	return;
}

//控制器事件回调
void UISInteractionComponent::ReciveControllerOpenUIEvent(APlayerController* InController)
{
	if(CurrentBuilding)
	{
		AISPlayerController* SourcePC = Cast<AISPlayerController>(InController);
		SourcePC->OnOpenInventoryEvent.RemoveDynamic(this,&UISInteractionComponent::ReciveControllerOpenUIEvent);
		PossessedObjectOnServer<AISBuildingSystemBase>(CurrentBuilding);
		CurrentBuilding = nullptr;
	}
}


//tick检测，实时检测当前准星面对的对象
void UISInteractionComponent::TickInteractline()
{
	AISCharacter* ISCharacter = Cast<AISCharacter>(GetOwner());
	if(!ISCharacter) return;
	
	AISPlayerController* SourcePC = Cast<AISPlayerController>( ISCharacter->GetController());  //获取角色控制器
	if(!SourcePC) return;
	AISPlayerMainHUD* PlayerMainHUD = Cast<AISPlayerMainHUD>(SourcePC->GetHUD()); //获取角色的HUD
	if(!PlayerMainHUD) return;  //程序有可能错误或者是开始游戏
	
	UCameraComponent*SourceCamera = ISCharacter->CameraComponent;
	FVector EyeLocation = SourceCamera->GetComponentLocation();
	FVector EyeForwardLocation = SourceCamera->GetForwardVector();
	FVector End = EyeLocation+(EyeForwardLocation*120);

	FHitResult Hit;
	if(GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,End,ECC_GameTraceChannel1))
	{
		if(Hit.GetActor() && Hit.GetActor()->Implements<UISBuildInterface>() && !bIsInteractTrace)
		{
			UISMainUIBase*ISMainUI = PlayerMainHUD->IsMainUI;  //获取角色的主UI
			if(!ISMainUI) return;
			LastBuildingActor =  IISBuildInterface::Execute_GetBuildingSystemBase(Hit.GetActor());  //获取检测到的建筑
			ISMainUI->SendObjectInfo(LastBuildingActor);
			bIsInteractTrace = true;
		}
		else if(Hit.GetActor() && Hit.GetActor()->Implements<UISEquipableInterface>() && !bIsInteractTrace)
		{
			UISMainUIBase*ISMainUI = PlayerMainHUD->IsMainUI;  //获取主UI
			if(!ISMainUI) return;
			AISEquipable* TargetEquipable = IISEquipableInterface::Execute_GetEquipable(Hit.GetActor());  //获取接口的对象
			ISMainUI->SendObjectInfo(TargetEquipable);
			bIsInteractTrace = true;
		}
		else if(Hit.GetActor() && Hit.GetActor()->Implements<UISNPCInterface>() && !bIsInteractTrace)
		{
			UISMainUIBase* ISMainUI = PlayerMainHUD->IsMainUI;
			if(!ISMainUI) return;
			AISNonePlayerCharacter* TargetNPC = IISNPCInterface::Execute_GetNPC(Hit.GetActor());
			if(!TargetNPC) return;
			ISMainUI->SendObjectInfo(TargetNPC);
			bIsInteractTrace = true;
		}
	}
	else
	{
		if(bIsInteractTrace)
		{
			UISMainUIBase*ISMainUI = PlayerMainHUD->IsMainUI;  //获取角色的主UI
			if(!ISMainUI) return;
			ISMainUI->SendTickTraceWasEnd();
			LastBuildingActor = nullptr;
			bIsInteractTrace = false;
		}
	}
}

void UISInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}
void UISInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickInteractline();
}

void UISInteractionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UISInteractionComponent,LastActor);
	DOREPLIFETIME(UISInteractionComponent,LastComponent);
}

template <typename T>
void UISInteractionComponent::PossessedObjectOnServer(T* InTarget)
{
	if(!InTarget) return;
	AISPlayerController* SourcePC = Cast<AISPlayerController>(UGameplayStatics::GetPlayerController(GetOwner(),0));
	InTarget->SetOwner(SourcePC);  //给予对方网络权限
}