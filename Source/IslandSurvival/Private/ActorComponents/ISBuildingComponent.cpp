// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISBuildingComponent.h"
#include "Character/ISCharacter.h"
#include "Math/MathFwd.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

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
	OnBuildingWasDestory.AddDynamic(this,&UISBuildingComponent::DestoryBuildPreviewOnClient);  //绑定删除建筑预览的广播事件
	
}


// Called every frame
void UISBuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceToMoveBuildPreview();
	// ...
}

//TargetIndex是为了保存当前你的快捷栏的快捷位置的索引
void UISBuildingComponent::BuildModeClient_Implementation(TSubclassOf<AISItemBase>BuildingSystemBase,const int32 TargetIndex)
{
	if(ISBuildingRef==nullptr&&bBuildPreviewWasCreated == false){SpawnBuildPreview(BuildingSystemBase);SaveHotBarIndex = TargetIndex;}  //预览建筑物不存在就新构建一个预览建筑物
	else{OnBuildingWasDestory.Broadcast();SaveHotBarIndex = -1;}  //在客户端中删除建筑预览，并清空Index
}

void UISBuildingComponent::SpawnBuildPreview(TSubclassOf<AISItemBase>BuildingSystemBase)
{
	ISBuildingRef = GetWorld()->SpawnActor<AISBuildingSystemBase>(BuildingSystemBase,ISBuildingTransformRef);  //生成Actor
	ISBuildingRef->ItemsStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bBuildPreviewWasCreated = true;
}
//追踪并移动建筑预览
void UISBuildingComponent::TraceToMoveBuildPreview_Implementation()
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
		
		TArray<AActor*>IgnoreActor;
		IgnoreActor.Add(GetOwner());
		IgnoreActor.Add(ISBuildingRef);
		FHitResult Hit;
	
		//如果预览建筑物存在，则不断地改变其位置
		if(UKismetSystemLibrary::LineTraceSingle(GetOwner(),CameraLocationAddDistanceClose,
		CameraForwardMultDistanceFar,ISBuildingRef->BuildingConfig.TraceType,
		false,IgnoreActor,EDrawDebugTrace::None,
		Hit,true,FLinearColor::Red,FLinearColor::Green,1.f))
		{
			FRotator CameraRotation = CameraComponent->GetComponentRotation();
			CameraRotation.Yaw += 90;
			FRotator BuildRotator(0,CameraRotation.Yaw,0);
			UE::Math::TQuat<double> Quat = FQuat(BuildRotator);  //旋转转换成四元数
			ISBuildingTransformRef.SetRotation(Quat);
			ISBuildingTransformRef.SetLocation(Hit.ImpactPoint);  //从撞击目标中获取目标位置

			if(ISBuildingRef)
			{
				AActor* ActorRef = Hit.GetActor();
				UActorComponent* ComponentRef = Cast<UActorComponent>(Hit.GetComponent());
				if(ActorRef&&ActorRef->Implements<UISBuildInterface>())
				{
					GetSnappingPoint(ActorRef,ComponentRef);
				}
				SetPreviewBuildingColor();
				
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
				AActor* ActorRef = Hit.GetActor();
				UActorComponent* ComponentRef = Cast<UActorComponent>(Hit.GetComponent());
				if(ActorRef&&ActorRef->Implements<UISBuildInterface>())
				{
					GetSnappingPoint(ActorRef,ComponentRef);
				}
				SetPreviewBuildingColor();
				ISBuildingRef->RootSceneComponent->SetWorldTransform(ISBuildingTransformRef);  //改变预览建筑物的位置
			}
		}
	}
}
void UISBuildingComponent::OneClickToDemoBuilding_Implementation(AActor* TargetActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("建筑损坏"));
	IISBuildInterface::Execute_DestoryBuilding(TargetActor,GetOwner());
}

void UISBuildingComponent::GetSnappingPoint(const AActor* TargetActor, UActorComponent* TargetComp)
{
	//循环遍历预览命中的建筑物的判定框
	for(auto BoxesRef:IISBuildInterface::Execute_GetBuildingBoxComponent(TargetActor))
	{
		if(BoxesRef==TargetComp)  //如果判定框等于击中的判定框，也就是同样的判定框，则返回该判定框的位置
		{
			/*若此时命中的组件（这个组件是建筑物中可衔接放置的命中框）在该建筑物中存在
			 * 则判定为可衔接放置建筑物
			 */
			ISBuildingTransformRef = BoxesRef->GetComponentTransform();
			IsAttaching = true;
			return;
		}
		IsAttaching = false;
	}
	return;
}

void UISBuildingComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UISBuildingComponent, Isfloating);
	DOREPLIFETIME(UISBuildingComponent,IsAttaching);
}



//删除建筑预览在客户端
void UISBuildingComponent::DestoryBuildPreviewOnClient_Implementation()
{
	ISBuildingRef->Destroy();
	ISBuildingRef = nullptr;
	bBuildPreviewWasCreated = false;
}
//在服务器中生成预览性建筑
void UISBuildingComponent::SpawnBuildOnServer_Implementation(TSubclassOf<AISItemBase>BuildingSystemBaseClass,FTransform Transform,bool bBuildingWasCreated)
{
	if(bBuildingWasCreated&&BuildingSystemBaseClass!=nullptr)
	{
		GetWorld()->SpawnActor<AISBuildingSystemBase>(BuildingSystemBaseClass,Transform);  //生成建筑
		OnBuildingWasDestory.Broadcast();
	}
}
//检查是否碰撞
bool UISBuildingComponent::CheckForOverlap()
{
	//查看建筑是否使用自定义重叠
	if(!ISBuildingRef) return false;
	FVector StaticOrigin = ISBuildingRef->ItemsStaticMesh->Bounds.Origin;
	FVector StaticExtent = ISBuildingRef->ItemsStaticMesh->Bounds.BoxExtent;
	FVector BoxOrigin = ISBuildingRef->BoxCollisionComponent->Bounds.Origin;
	FVector BoxExtent = ISBuildingRef->BoxCollisionComponent->Bounds.BoxExtent;
	float HalfSizeFloat = 1.2f;
	FRotator RootSceneRotation =  ISBuildingRef->RootSceneComponent->GetComponentRotation();
	RootSceneRotation.Yaw += 90;
	TArray<AActor*>IgnoreActor;
	IgnoreActor.Add(GetOwner());
	IgnoreActor.Add(ISBuildingRef);
	FHitResult Hit;
	if(ISBuildingRef->BuildingConfig.UseCustomOverlap)  //使用自定义堆叠，则使用建筑的碰撞箱进行碰撞检测
	{
		FVector HalfSize = BoxExtent/HalfSizeFloat;
		return UKismetSystemLibrary::BoxTraceSingle(GetOwner(),BoxOrigin,BoxOrigin,HalfSize,FRotator(0,RootSceneRotation.Yaw,0),TraceTypeQuery1,true
			,IgnoreActor,EDrawDebugTrace::None,Hit,true,FLinearColor::Red,FLinearColor::Green,1.f);
	}
	else  //不适用自定义堆叠，则使用建筑的模型碰撞箱进行碰撞检测
	{
		FVector HalfSize = StaticExtent/HalfSizeFloat;
		return UKismetSystemLibrary::BoxTraceSingle(GetOwner(),StaticOrigin,StaticOrigin,HalfSize,FRotator(0,RootSceneRotation.Yaw,0),TraceTypeQuery1,true
			,IgnoreActor,EDrawDebugTrace::None,Hit,true,FLinearColor::Red,FLinearColor::Green,1.f);
	}
}

//检查建筑物是否浮空
bool UISBuildingComponent::CheckBuildFloating()
{
	if(!ISBuildingRef) return false;
	FVector StaticOrigin = ISBuildingRef->ItemsStaticMesh->Bounds.Origin;
	FVector StaticExtent = ISBuildingRef->ItemsStaticMesh->Bounds.BoxExtent;

	float HalfSizeFloat = 1.2f;
	FRotator RootSceneRotation =  ISBuildingRef->RootSceneComponent->GetComponentRotation();
	RootSceneRotation.Yaw += 90;
	
	TArray<AActor*>IgnoreActor;
	IgnoreActor.Add(GetOwner());
	IgnoreActor.Add(ISBuildingRef);
	TArray<FHitResult>  Hits;

	TArray<TEnumAsByte<EObjectTypeQuery>> BuildingObjectTypes;
	BuildingObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	FVector HalfSize = StaticExtent/HalfSizeFloat;

	return ( UKismetSystemLibrary::BoxTraceMulti(GetOwner(),StaticOrigin,StaticOrigin,HalfSize,FRotator(0,RootSceneRotation.Yaw,0),
		TraceTypeQuery18,false,IgnoreActor,EDrawDebugTrace::None,Hits,true,FLinearColor::Red,FLinearColor::Green,1.f));
}
//检查建筑物是否放置在地基上
bool UISBuildingComponent::CheckBuildOnFoundation()
{
	if(!ISBuildingRef) return false;
	FVector EndLocation = ISBuildingTransformRef.GetLocation();
	EndLocation-=FVector(0,0,50);
	TArray<AActor*>IgnoreActor;
	IgnoreActor.Add(GetOwner());
	IgnoreActor.Add(ISBuildingRef);
	FHitResult Hit;
	if(UKismetSystemLibrary::LineTraceSingle(GetOwner(),ISBuildingTransformRef.GetLocation(),EndLocation,ETraceTypeQuery::TraceTypeQuery1,true,IgnoreActor
	,EDrawDebugTrace::ForDuration,Hit,true,FLinearColor::Red,FLinearColor::Green,1.f))
	{
		if(!Hit.GetActor()->Implements<UISBuildInterface>()) return false;
		return true;
	}
	return false;
}
//设置预览建筑物的颜色
void UISBuildingComponent::SetPreviewBuildingColor()
{
	if(!ISBuildingRef) return;
	
	FISBuildBooleanCheck BuildBooleanCheck(ISBuildingRef->ItemsStaticMesh,0,ISBuildingRef->BuildingConfig.DoFloatCheck,CheckForOverlap(),CheckBuildFloating(),
		ISBuildingRef->BuildingConfig.CanPlaceOnFoundation,CheckBuildOnFoundation());
	OnCallSetMaterial.Broadcast(BuildBooleanCheck);
}


bool UISBuildingComponent::CanBuildCheck()
{
	if(!ISBuildingRef) return false;
	//如果建筑物需要做浮空检查
	if(ISBuildingRef->BuildingConfig.DoFloatCheck)
	{
		//如果放置在地面
		if(CheckBuildFloating())
		{
			//如果连接
			if(IsAttaching)
			{
				if(!ISBuildingRef->BuildingConfig.bIsNeedToCheckOverlap)
				{
					return true;
				}
				if(ISBuildingRef->BuildingConfig.bIsNeedToCheckOverlap && !CheckForOverlap())
				{
					return true;
				}
			}
			//如果没有连接
			else
			{
				if(ISBuildingRef->BuildingConfig.BuildingType == Foundation)
				{
					return false;
				}
				if(!ISBuildingRef->BuildingConfig.bIsNeedToCheckOverlap)
				{
					return true;
				}
				if(ISBuildingRef->BuildingConfig.bIsNeedToCheckOverlap && !CheckForOverlap())
				{
					return true;
				}
			}
		}
		//如果浮空
		else
		{
			if(IsAttaching)
			{
				if(!ISBuildingRef->BuildingConfig.bIsNeedToCheckOverlap)
				{
					return true;
				}
				if(ISBuildingRef->BuildingConfig.bIsNeedToCheckOverlap && ! CheckForOverlap())
				{
					return true;
				}
			}
			else
			{
				if(ISBuildingRef->BuildingConfig.BuildingType==Foundation && CheckBuildOnFoundation())
				{
					return true;
				}
			}
		}
	}
	//如果不需要做浮空检查
	else
	{
		if(IsAttaching)
		{
			if(!ISBuildingRef->BuildingConfig.bIsNeedToCheckOverlap)
			{
				return true;
			}
			if(ISBuildingRef->BuildingConfig.bIsNeedToCheckOverlap && ! CheckForOverlap())
			{
				return true;
			}
		}
	}
	return false;
}
