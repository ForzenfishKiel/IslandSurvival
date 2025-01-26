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

	/*用于序列化类的参数*/
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** 创建一个副本，用于后续网络复制或者后续修改 */
	virtual FISGameplayEffectContext* Duplicate() const override
	{
		FISGameplayEffectContext* NewContext = new FISGameplayEffectContext();
		*NewContext = *this;//WithCopy 设置为true，就可以通过赋值操作进行拷贝
		if (GetHitResult())
		{
			// 深拷贝 hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
protected:
	UPROPERTY()
	FName TargetSaleID = FName();  //交易的ID
};
template<>
struct TStructOpsTypeTraits< FISGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FISGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
