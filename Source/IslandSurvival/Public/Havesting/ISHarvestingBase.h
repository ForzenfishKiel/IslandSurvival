// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ISCollectibleInterface.h"
#include "ISHarvestingBase.generated.h"

UCLASS()
class ISLANDSURVIVAL_API AISHarvestingBase : public AActor,public IISCollectibleInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AISHarvestingBase();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> HarvestStaticMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ECollectibleClass CollectibleClass = ECollectibleClass::None;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Config")
	float CollectibleHP = 0.f;  //采集物血量
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Config")
	float CollectibleMaxHP = 100.f; //采集物最大血量
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Config")
	FName CollectibleName;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual FName GetTargetName() override;
	virtual ECollectibleClass GetTargetClass_Implementation();
};
