// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISInteractionComponent.h"
#include "Character/ISCharacter.h"
#include "Game/ISPlayerController.h"
#include "Game/ISPlayerMainHUD.h"
#include "Net/UnrealNetwork.h"
#include "Interface/ISItemInterface.h"

// Sets default values for this component's properties
UISInteractionComponent::UISInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

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
				IISBuildInterface::Execute_OnBuildingWasInteract(CompOwner, GetOwner(), HitComponent);
				return;
			}
			else if(HitActor->Implements<UISBuildInterface>())
			{
				IISBuildInterface::Execute_OnBuildingWasInteract(HitActor, GetOwner(), HitComponent);
				return;
			}
		}
	}
}
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
		if(Hit.GetActor()->Implements<UISBuildInterface>()&&!bIsInteractTrace)
		{
			UISMainUIBase*ISMainUI = PlayerMainHUD->IsMainUI;  //获取角色的主UI
			if(!ISMainUI) return;
			LastBuildingActor =  IISBuildInterface::Execute_GetBuildingSystemBase(Hit.GetActor());  //获取检测到的建筑
			ISMainUI->SendBuildingInfo(LastBuildingActor);
			bIsInteractTrace = true;
		}
		if(Hit.GetActor()->Implements<UISItemInterface>())
		{
			
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