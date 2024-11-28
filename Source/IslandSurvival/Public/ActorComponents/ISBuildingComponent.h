// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/ISBuildingSystemBase.h"
#include "Components/ActorComponent.h"
#include "ISBuildingComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingWasDestory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCallSetMaterial,UStaticMeshComponent*,BuildMesh,int32,TargetIndex,bool,bIsOverlap);
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
	bool CheckForOverlap();
	UFUNCTION(Server,Reliable)
	void SpawnBuildOnServer(TSubclassOf<AISItemBase>BuildingSystemBaseClass,FTransform Transform,bool bBuildingWasCreated);
	UFUNCTION(Client,Reliable)
	void DestoryBuildPreviewOnClient();
	FTransform GetSnappingPoint(const AActor* TargetActor,UActorComponent*TargetComp);
	UPROPERTY(BlueprintAssignable)
	FOnBuildingWasDestory OnBuildingWasDestory;
	UPROPERTY(BlueprintAssignable)
	FOnCallSetMaterial OnCallSetMaterial;
	UPROPERTY()
	TObjectPtr<AISBuildingSystemBase>ISBuildingRef;  //建筑单例的引用
	FTransform ISBuildingTransformRef = FTransform();
	FVector ISBuildCenter = FVector();
	bool bBuildPreviewWasCreated = false;
	int32 SaveHotBarIndex = -1;
};
