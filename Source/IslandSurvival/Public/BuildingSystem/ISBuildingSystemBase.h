// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Interface/ISBuildInterface.h"
#include "Items/ISItemBase.h"
#include "ISBuildingSystemBase.generated.h"


UENUM(BlueprintType)
enum EISBuildingType
{
	None = 0 UMETA(DisplayName = "None"),
	Foundation  = 1 UMETA(DisplayName = "Foundation"),
	AboveFoundation = 2 UMETA(DisplayName = "AboveFoundation"),
};
USTRUCT(BlueprintType)
struct FISBuildingConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName BuildName = FName("None");  //建筑物名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ETraceTypeQuery> TraceType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EISBuildingType> BuildingType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool CanPlaceOnGround = false;  //是否可以放置在地上
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool UseCustomOverlap = false;  //自定义堆叠
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool DoFloatCheck = false;//是否可以漂浮在空中
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool CanPlaceOnFoundation = false;  //锻造的物体是否可以被放置，既可以放在地基上也可以放在物体上，类似营火
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool DoCeilingCheck = false;  //是否可以放置在天花板
};

UCLASS()
class ISLANDSURVIVAL_API AISBuildingSystemBase : public AISItemBase,public IISBuildInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AISBuildingSystemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Replicated)
	int32 BuildHP = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Replicated)
	int32 BuildMaxHP = 100;  //建筑物最大血量

	UFUNCTION(BlueprintImplementableEvent)
	void EnableBuildBroken(AActor* TargetActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> RootSceneComponent;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxCollisionComponent;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Config")
	FISBuildingConfig BuildingConfig;  //保存建筑信息配置

	
	virtual AISBuildingSystemBase* GetBuildingSystemBase_Implementation() override;

	
	virtual TArray<UBoxComponent*> GetBuildingBoxComponent_Implementation() const override;

	
	virtual void OnBuildingWasInteract_Implementation(const AActor* InteractingActor, const UActorComponent* InteractingComponent) override;

	
	virtual void OnBuildBroking_Implementation(AActor* TargetActor) override;

	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	
	virtual void DestoryBuilding_Implementation(AActor*TargetActor) override;
	
	UFUNCTION(Server,Reliable)
	virtual void InteractOnServer(AController* InPlayerController);
};
