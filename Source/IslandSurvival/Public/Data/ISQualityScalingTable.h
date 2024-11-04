// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ISQualityScalingTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEquipableQuanlityScaling
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Informations")
	FName TargetID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Informations")
	float Common = 0.f;  //常见的
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Informations")
	float UnCommon = 0.f; //不常见的
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Informations")
	float Rare = 0.f;  //稀有的
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Informations")
	float Epic = 0.f;
};
UCLASS()
class ISLANDSURVIVAL_API UISQualityScalingTable : public UObject
{
	GENERATED_BODY()
	
};
