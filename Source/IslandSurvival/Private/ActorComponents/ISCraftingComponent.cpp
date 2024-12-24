// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISCraftingComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Character/ISCharacter.h"
#include "Game/ISAttributeSet.h"
#include "Game/ISGameInstance.h"
#include "Game/ISGameplayTagsManager.h"
#include "Game/ISPlayerState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UISCraftingComponent::UISCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

int32 UISCraftingComponent::FindCheckCharacterBackPack(const UDataTable* TargetDT, const int32 TargetID,
	const int32 RequireID)
{
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetOwner());
	UISHotBarInventory*CharacterHotBar = SourceCharacter->GetComponentByClass<UISHotBarInventory>();
	UISCharacterInventory*CharacterInventory = SourceCharacter->GetComponentByClass<UISCharacterInventory>();
	
	//本地运行
	UISGameInstance*ISGameplayInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));  //获取本地游戏实例
	if(!ISGameplayInstance) return 0;
	UISCraftingContainer*CraftingContainer = ISGameplayInstance->ItemContainer;
	if(!CraftingContainer) return 0;
	int32 Result = 0;
	FName Trans = FName(FString::Printf(TEXT("%d"),TargetID));
	
	if(FItemRecipe*UserInfo = TargetDT->FindRow<FItemRecipe>(Trans,TEXT("name")))
	{
		for(auto&DataTableRef:UserInfo->ItemRequired)
		{
			if(DataTableRef.ItemID!=RequireID) continue;
			for(int32 Index = 0;Index<CharacterHotBar->InventoryContainer.Num();Index++)
			{
				if(CharacterHotBar->InventoryContainer[Index].ItemID==RequireID)
				{
					const int32 Value = CharacterHotBar->InventoryContainer[Index].ItemQuantity;
					Result += Value;
				}
			}
			for(int32 Index = 0;Index<CharacterInventory->InventoryContainer.Num();Index++)
			{
				if(CharacterInventory->InventoryContainer[Index].ItemID==RequireID)
				{
					const int32 Value = CharacterInventory->InventoryContainer[Index].ItemQuantity;
					Result += Value;
				}
			}
			return Result;
		}
	}
	return 0;
}

/*检查是否可以制造
 * 优先检索角色物品栏，然后是背包，检查角色库存内是否有需要制作的物品所需要的材料，如果有则返回True，反之
 * 对满足的物品的数量进行累加，如果数量不够，则返回false ， 反之
 */
bool UISCraftingComponent::IsCanBeCrafting(const UDataTable* TargetDT, const int32 TargetID, const int32 RequireID)
{
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetOwner());
	UISHotBarInventory*CharacterHotBar = SourceCharacter->GetComponentByClass<UISHotBarInventory>();
	UISCharacterInventory*CharacterInventory = SourceCharacter->GetComponentByClass<UISCharacterInventory>();
	
	//本地运行
	UISGameInstance*ISGameplayInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));  //获取本地游戏实例
	if(!ISGameplayInstance) return false;
	UISCraftingContainer*CraftingContainer = ISGameplayInstance->ItemContainer;
	if(!CraftingContainer) return false;
	int32 Result = 0;
	FName Trans = FName(FString::Printf(TEXT("%d"),TargetID));
	
	if(FItemRecipe*UserInfo = TargetDT->FindRow<FItemRecipe>(Trans,TEXT("name")))
	{
		for(auto&DataTableRef:UserInfo->ItemRequired)
		{
			if(DataTableRef.ItemID!=RequireID) continue;
			for(int32 Index = 0;Index<CharacterHotBar->InventoryContainer.Num();Index++)
			{
				if(CharacterHotBar->InventoryContainer[Index].ItemID==RequireID)
				{
					const int32 Value = CharacterHotBar->InventoryContainer[Index].ItemQuantity;
					Result += Value;
				}
			}
			for(int32 Index = 0;Index<CharacterInventory->InventoryContainer.Num();Index++)
			{
				if(CharacterInventory->InventoryContainer[Index].ItemID==RequireID)
				{
					const int32 Value = CharacterInventory->InventoryContainer[Index].ItemQuantity;
					Result += Value;
				}
			}
			if(Result>=DataTableRef.ItemQuantity)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

void UISCraftingComponent::CraftingAction(const UDataTable*TargetDT,const int32 TargetID)
{
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetOwner());
	UISHotBarInventory*CharacterHotBar = SourceCharacter->GetComponentByClass<UISHotBarInventory>();  //获取物品栏
	UISCharacterInventory*CharacterInventory = SourceCharacter->GetComponentByClass<UISCharacterInventory>();  //获取背包


	FName Trans = FName(FString::Printf(TEXT("%d"),TargetID));
	int32 Result = 0;
	FItemRecipe*UserInfo = TargetDT->FindRow<FItemRecipe>(Trans,TEXT("name"));
	if(UserInfo)
	{
		for(auto&DataTableRef:UserInfo->ItemRequired)
		{
			for(int32 Index = 0;Index<CharacterHotBar->InventoryContainer.Num();Index++)
			{
				//优先搜索角色的物品栏
				if(CharacterHotBar->InventoryContainer[Index].ItemID==DataTableRef.ItemID)
				{
					const int32 Value = CharacterHotBar->InventoryContainer[Index].ItemQuantity;
					Result+=Value;
					if(Result==DataTableRef.ItemQuantity){CharacterHotBar->DiscardItem(Index,Result);  Result = 0; break;}
					if(Result>DataTableRef.ItemQuantity){CharacterHotBar->DiscardItem(Index,DataTableRef.ItemQuantity); Result = 0; break;}
					CharacterHotBar->DiscardItem(Index,Value);
				}
			}
			for(int32 Index = 0;Index<CharacterInventory->InventoryContainer.Num();Index++)
			{
				//再搜索角色的背包
				if(CharacterInventory->InventoryContainer[Index].ItemID==DataTableRef.ItemID)
				{
					const int32 Value = CharacterInventory->InventoryContainer[Index].ItemQuantity;
					Result+=Value;
					if(Result==DataTableRef.ItemQuantity){CharacterInventory->DiscardItem(Index,Result);  Result = 0; break;}
					if(Result>DataTableRef.ItemQuantity){CharacterInventory->DiscardItem(Index,DataTableRef.ItemQuantity); Result = 0; break;}
					
					CharacterInventory->DiscardItem(Index,Value);
				}
			}
		}

	}
	return;
}

void UISCraftingComponent::AddToCharacterBackPack_Implementation(const UDataTable* TargetDT, const int32 TargetID)
{
	AISCharacter*SourceCharacter = Cast<AISCharacter>(GetOwner());
	if(!SourceCharacter) return;
	UISItemsContainer*ItemsContainer = SourceCharacter->GetComponentByClass<UISItemsContainer>();  //获取系统库存组件
	
	FName Trans = FName(FString::Printf(TEXT("%d"),TargetID));
	//(暂定)制作完成后将制作完成的物品添加到角色
	ItemsContainer->PickUpItemForID(SourceCharacter,Trans,1);

	if(FItemRecipe*UserInfo = TargetDT->FindRow<FItemRecipe>(Trans,TEXT("name")))
	{
		SendXPToTarget(UserInfo->ItemExperience);  //传输制造获得的经验值
	}
}
//发送给角色相应的经验值
void UISCraftingComponent::SendXPToTarget_Implementation(float TargetXP)
{
	if(TargetXP==0) return;
	UGameplayEffect*Effect = NewObject<UGameplayEffect>(GetTransientPackage(),FName("CraftingXP")); //创建一个临时的GE变量
	Effect->DurationPolicy = EGameplayEffectDurationType::Instant;  //顺时地传输
	Effect->PeriodicInhibitionPolicy = EGameplayEffectPeriodInhibitionRemovedPolicy::NeverReset;//设置每次应用不会重置触发时间
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource; //聚合为原自身
	Effect->StackLimitCount = 1;  //设置可堆的栈上限为1，防止栈溢出
	Effect->StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::ClearEntireStack;  //当应用效果完毕后，栈会清除

	UTargetTagsGameplayEffectComponent&TargetTagsGameplayEffectComponent = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer = TargetTagsGameplayEffectComponent.GetConfiguredTargetTagChanges();
	FGameplayTagsManager GameplayTagsManager = FGameplayTagsManager::Get();  //从我们的标签库中获取对象
	InheritedTagContainer.AddTag(GameplayTagsManager.Attribute_Meta_InComingXP);  //添加标签为获取经验值
	TargetTagsGameplayEffectComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	const int32 Index = Effect->Modifiers.Num();  //获取当前修改属性Modifiers的长度，也就是最新的那一位
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo&ModifierInfo = Effect->Modifiers[Index];  //通过下标索引获取对应的Modifier
	ModifierInfo.ModifierMagnitude = FScalableFloat(TargetXP);
	ModifierInfo.ModifierOp = EGameplayModOp::Override;  //设置属性运算符号
	ModifierInfo.Attribute = UISAttributeSet::GetInComingXPAttribute();  //指定对应的属性

	AISCharacter* SourceCharacter = Cast<AISCharacter>(GetOwner());
	AISPlayerState* SourcePlayerState = SourceCharacter->GetPlayerState<AISPlayerState>();
	check(SourcePlayerState);
	UAbilitySystemComponent*SourceASC = SourcePlayerState->GetAbilitySystemComponent();
	FGameplayEffectContextHandle GameplayEffectContextHandle = SourceASC->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this); //添加效果的来源为自身
	if(const FGameplayEffectSpec*MutableSpec = new FGameplayEffectSpec(Effect,GameplayEffectContextHandle,1.f))
	{
		SourceASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);  //应用效果
	}
}
// Called when the game starts
void UISCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UISCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

