// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Havesting/ISHarvestingBase.h"
#include "ISCorPseHarvestBase.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISCorPseHarvestBase : public AISHarvestingBase
{
	GENERATED_BODY()
public:
	AISCorPseHarvestBase();
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> CorPseSkeletalMesh;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<USceneComponent> CorPseRoot;
};
