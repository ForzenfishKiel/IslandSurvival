#pragma once

#include "GameplayEffectTypes.h"
#include "ISAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FISGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	FName GetTargetSaleID() const { return TargetSaleID;}
	void SetTargetSaleID(const FName InNewID) {	TargetSaleID = InNewID; }

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
protected:
	UPROPERTY()
	FName TargetSaleID = FName();  //交易的ID
};