// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISBuildingComponent.h"
#include "Character/ISCharacter.h"
#include "Math/MathFwd.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UISBuildingComponent::UISBuildingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UISBuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UISBuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceToMoveBuildPreview();
	// ...
}

void UISBuildingComponent::BuildModeClient_Implementation(TSubclassOf<AISItemBase>BuildingSystemBase)
{
	if(ISBuildingRef==nullptr&&bBuildPreviewWasCreated == false){SpawnBuildPreview(BuildingSystemBase);}  //预览建筑物不存在就新构建一个预览建筑物
	else{ISBuildingRef->Destroy();ISBuildingRef = nullptr;bBuildPreviewWasCreated = false;}
}

void UISBuildingComponent::SpawnBuildPreview(TSubclassOf<AISItemBase>BuildingSystemBase)
{
	ISBuildingRef = GetWorld()->SpawnActor<AISBuildingSystemBase>(BuildingSystemBase,ISBuildingTransformRef);  //生成Actor
	UStaticMeshComponent*BuildStaticMeshComponent = ISBuildingRef->ItemsStaticMesh;  //获取静态网格体
	BuildStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bBuildPreviewWasCreated = true;
}

void UISBuildingComponent::TraceToMoveBuildPreview()
{
	if(bBuildPreviewWasCreated)
	{
		int32 BuildDistanceClose = 350;
		int32 BuildDistanceFar = 1000;
		AISCharacter* CharacterRef = Cast<AISCharacter>(GetOwner()); //获取本地角色
		UCameraComponent* CameraComponent = CharacterRef->FindComponentByClass<UCameraComponent>();  //获取角色的摄像机
		FVector CameraLocation = CameraComponent->GetComponentLocation();  //从世界中获取组件的位置
		FVector CameraForwardVector = CameraComponent->GetForwardVector();  //获取摄像机的向前向量
	
		FVector CameraLocationAddDistanceClose =  (CameraForwardVector * BuildDistanceClose) + CameraLocation;

		FVector CameraForwardMultDistanceFar = (CameraForwardVector * BuildDistanceFar) + CameraLocation;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
		ObjectTypeQuery.Add(EObjectTypeQuery::ObjectTypeQuery1);
		TArray<AActor*>IgnoreActor;
		IgnoreActor.Add(GetOwner());
		FHitResult Hit;
	
		//如果预览建筑物存在，则不断地改变其位置
		if(UKismetSystemLibrary::LineTraceSingleForObjects(GetOwner(),CameraLocationAddDistanceClose,
		CameraForwardMultDistanceFar,ObjectTypeQuery,
		false,IgnoreActor,EDrawDebugTrace::ForDuration,
		Hit,true,FLinearColor::Yellow,FLinearColor::Green,1.f))
		{
			FRotator CameraRotation = CameraComponent->GetComponentRotation();
			CameraRotation.Yaw += 90;
			FRotator BuildRotator(0,CameraRotation.Yaw,0);
			UE::Math::TQuat<double> Quat = FQuat(BuildRotator);  //旋转转换成四元数
			ISBuildingTransformRef.SetRotation(Quat);
			ISBuildingTransformRef.SetLocation(Hit.ImpactPoint);  //从撞击目标中获取目标位置

			if(ISBuildingRef)
			{
				ISBuildingRef->RootSceneComponent->SetWorldTransform(ISBuildingTransformRef);  //改变预览建筑物的位置
			}
		}
		//如果未获取到撞击则一直持续更新变量位置
		else
		{
			FRotator CameraRotation = CameraComponent->GetComponentRotation();
			CameraRotation.Yaw += 90;
			FRotator BuildRotator(0,CameraRotation.Yaw,0);
			UE::Math::TQuat<double> Quat = FQuat(BuildRotator);  //旋转转换成四元数
			ISBuildingTransformRef.SetRotation(Quat);
			ISBuildingTransformRef.SetLocation(Hit.TraceEnd);  //从撞击目标中获取目标位置
			if(ISBuildingRef)
			{
				ISBuildingRef->RootSceneComponent->SetWorldTransform(ISBuildingTransformRef);  //改变预览建筑物的位置
			}
		}
	}
}

//设置预览建筑物的颜色
void UISBuildingComponent::SetPreviewBuildingColor()
{
	if(!ISBuildingRef) return;
	for(auto&CompRef:ISBuildingRef->GetComponents())
	{
		//尝试遍历该预览建筑的静态网格体的所有材质
		UStaticMeshComponent*BuildingStaticMesh = Cast<UStaticMeshComponent>(CompRef);
		if(BuildingStaticMesh)
		{
			for(int32 i = 0;i<BuildingStaticMesh->GetNumMaterials();i++)
			{
				BuildingStaticMesh->SetMaterial(i,BuildingStaticMesh->GetMaterial(i));  //设置静态网格体的材质
			}
		}
	}
}

