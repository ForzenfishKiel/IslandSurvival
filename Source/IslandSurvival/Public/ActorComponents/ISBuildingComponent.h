// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/ISBuildingSystemBase.h"
#include "Components/ActorComponent.h"
#include "ISBuildingComponent.generated.h"



USTRUCT(BlueprintType)
struct FISBuildBooleanCheck
{
	GENERATED_BODY()
public:
	FISBuildBooleanCheck() {}
	FISBuildBooleanCheck(UStaticMeshComponent* InBuildMesh, int32 InBuildIndex, bool InbDoFloatCheck,bool InbIsOverlapping,bool InbISFloating,bool bCanBeBuildOnFoundation,bool bCheckBuildOnFoundation):
	BuildMesh(InBuildMesh),TargetIndex(InBuildIndex),bDoFloatCheck(InbDoFloatCheck),bIsOverlap(InbIsOverlapping),bIsFloating(InbISFloating),bCanBeBuildOnFoundation(bCanBeBuildOnFoundation)
	,bCheckBuildFoundation(bCheckBuildOnFoundation){}
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* BuildMesh = nullptr;
	UPROPERTY(BlueprintReadOnly)
	int32 TargetIndex = -1;
	UPROPERTY(BlueprintReadOnly)
	bool bDoFloatCheck = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsOverlap = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsFloating = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanBeBuildOnFoundation = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCheckBuildFoundation = false;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingWasDestory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCallSetMaterial,FISBuildBooleanCheck,BuildBooleanCheck);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISLANDSURVIVAL_API UISBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UISBuildingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable,Client,Reliable)
	void BuildModeClient(TSubclassOf<AISItemBase>BuildingSystemBase,const int32 TargetIndex);
	void SpawnBuildPreview(TSubclassOf<AISItemBase>BuildingSystemBase);
	UFUNCTION(Client,Reliable)
	void TraceToMoveBuildPreview();
	void SetPreviewBuildingColor();
	bool CheckForOverlap();  //检查预览建筑物是否发生了碰撞
	bool CheckBuildFloating();//检查预览建筑物是否在地面
	bool CheckBuildOnFoundation(); //检查建筑物是否只能被放置于地基
	UFUNCTION(Server,Reliable)
	void SpawnBuildOnServer(TSubclassOf<AISItemBase>BuildingSystemBaseClass,FTransform Transform,bool bBuildingWasCreated);
	UFUNCTION(Client,Reliable)
	void DestoryBuildPreviewOnClient();
	UFUNCTION(Server,Reliable)
	void OneClickToDemoBuilding(AActor* TargetActor);
	void GetSnappingPoint(const AActor* TargetActor,UActorComponent*TargetComp);
	UPROPERTY(BlueprintAssignable)
	FOnBuildingWasDestory OnBuildingWasDestory;
	UPROPERTY(BlueprintAssignable)
	FOnCallSetMaterial OnCallSetMaterial;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AISBuildingSystemBase>ISBuildingRef;  //建筑单例的引用
	FTransform ISBuildingTransformRef = FTransform();
	FVector ISBuildCenter = FVector();
	bool bBuildPreviewWasCreated = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bCanBeBuild = false;
	int32 SaveHotBarIndex = -1;

	UFUNCTION(BlueprintCallable)
	bool FloatingCheck();

	
private:
	UPROPERTY(Replicated)
	bool Isfloating = false;
	UPROPERTY(Replicated)
	bool IsAttaching = false;

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
