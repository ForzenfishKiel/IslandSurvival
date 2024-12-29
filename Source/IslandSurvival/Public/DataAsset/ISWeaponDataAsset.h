// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ISWeaponDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FName Fp_AttachSocket;
		
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FName Tp_AttachSocket;
};
UCLASS(BlueprintType)
class ISLANDSURVIVAL_API UISWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	FWeaponInfo GetWeaponInfo(const int32 TargetID);
private:
	UPROPERTY(EditDefaultsOnly,Category = "Default")
	TMap<int32,FWeaponInfo> WeaponInfoMap;
};
