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
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated,Category = "Config")
	ECollectibleClass CollectibleClass = ECollectibleClass::None;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float CollectibleMaxHP = 100.f; //采集物最大血量
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated,Category = "Config")
	float CollectibleHP = 0.f;  //采集物血量
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated,Category = "Config")
	FName CollectibleName;  //采集物名字

	UFUNCTION(BlueprintImplementableEvent)
	void EnableHarvestionBreak(AActor*TargetActor);
	UFUNCTION(BlueprintImplementableEvent)
	void CallMulticastDelegates();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual FName GetTargetName() override;
	virtual ECollectibleClass GetTargetClass_Implementation();
	virtual void CollectionExecution_Implementation(AActor* TargetActor, AActor* TargetTool) override;
	virtual void ApplyDamageToTarget_Implementation(AActor* TargetActor) override;
	virtual int32 GetNumsFromMultiplier_Implementation(AActor* TargetTool,int32 TargetNums) override;
};
