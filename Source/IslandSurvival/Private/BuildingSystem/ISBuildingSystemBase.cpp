// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/ISBuildingSystemBase.h"

#include "Character/ISCharacter.h"
#include "Game/ISAttributeSet.h"
#include "Game/ISPlayerState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AISBuildingSystemBase::AISBuildingSystemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PickUpCheckSphere->UnregisterComponent();
	PickUpCheckSphere->DestroyComponent();
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);
	ItemsStaticMesh->SetupAttachment(RootSceneComponent);
	ItemsStaticMesh->SetCollisionObjectType(ECC_Destructible);
	RootSceneComponent->SetMobility(EComponentMobility::Movable);
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	BoxCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);//所有通道全忽略
	BoxCollisionComponent->SetupAttachment(RootSceneComponent);
	bReplicates = true;
}

AISBuildingSystemBase* AISBuildingSystemBase::GetBuildingSystemBase_Implementation()
{
	return this;
}

void AISBuildingSystemBase::InitItemConfig()
{
	Super::InitItemConfig();
	BuildHP = BuildMaxHP;
}

//获取建筑的吸附判定框（数组）
TArray<UBoxComponent*> AISBuildingSystemBase::GetBuildingBoxComponent_Implementation() const
{
	TArray<UBoxComponent*> ISBoxComponent;
	for(auto&CompRef:GetComponents())
	{
		UStaticMeshComponent*BuildMeshRef = Cast<UStaticMeshComponent>(CompRef);
		if(BuildMeshRef)
		{
			for(auto&StaticMeshChildRef:BuildMeshRef->GetAttachChildren())
			{
				UBoxComponent* BoxCompRef = Cast<UBoxComponent>(StaticMeshChildRef);
				if(BoxCompRef)
				{
					ISBoxComponent.Add(BoxCompRef);
				}
			}
		}
	}
	return ISBoxComponent;
}

void AISBuildingSystemBase::OnBuildingWasInteract_Implementation(const AActor* InteractingActor,
	const UActorComponent* InteractingComponent)
{
	IISBuildInterface::OnBuildingWasInteract_Implementation(InteractingActor, InteractingComponent);
}

void AISBuildingSystemBase::OnBuildBroking_Implementation(AActor* TargetActor)
{
	AISCharacter* SourceCharacter = Cast<AISCharacter>(TargetActor);
	if(!SourceCharacter) return;
	AISPlayerState* SourcePlayerState = Cast<AISPlayerState>(SourceCharacter->GetPlayerState<AISPlayerState>());  //获取角色状态类
	if(!SourcePlayerState) return;

	UAbilitySystemComponent* SourceASC = Cast<UAbilitySystemComponent>(SourcePlayerState->GetAbilitySystemComponent());  //获取角色ASC组件
	if(!SourceASC) return;

	UISAttributeSet* SourceAS = Cast<UISAttributeSet>(SourcePlayerState->GetAttributeSet()); //获取角色属性
	
	const float WeaponAttack = SourceAS->GetWeaponAttack();
 
	if(WeaponAttack>0)
	{
		BuildHP = FMath::Clamp(BuildHP-WeaponAttack,0.f,BuildMaxHP);
	}
	if(BuildHP<=0)
	{
		EnableBuildBroken(TargetActor);  //应用建筑物损坏
	}
}
void AISBuildingSystemBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AISBuildingSystemBase,BuildHP);
	DOREPLIFETIME(AISBuildingSystemBase,BuildMaxHP);
}

void AISBuildingSystemBase::DestoryBuilding_Implementation(AActor*TargetActor)
{
	EnableBuildBroken(TargetActor);
}

void AISBuildingSystemBase::InteractOnServer_Implementation(AController* InPlayerController)
{
	
}
bool AISBuildingSystemBase::ShouldLoadTransform_Implementation()
{
	return IISSaveInterface::ShouldLoadTransform_Implementation();
}

void AISBuildingSystemBase::LoadActor_Implementation()
{
	IISSaveInterface::LoadActor_Implementation();
}