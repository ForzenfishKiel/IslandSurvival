// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/ISBuildingSystemBase.h"
#include "Components/ActorComponent.h"
#include "ISBuildingComponent.generated.h"


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
	void BuildModeClient(TSubclassOf<AISItemBase>BuildingSystemBase);
	void SpawnBuildPreview(TSubclassOf<AISItemBase>BuildingSystemBase);
	void TraceToMoveBuildPreview();
	void SetPreviewBuildingColor();
	UPROPERTY()
	TObjectPtr<AISBuildingSystemBase>ISBuildingRef;  //建筑单例的引用
	UPROPERTY()
	FTransform ISBuildingTransformRef = FTransform();
	bool bBuildPreviewWasCreated = false;
};
