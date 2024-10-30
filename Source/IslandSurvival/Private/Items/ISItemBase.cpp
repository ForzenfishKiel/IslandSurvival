// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ISItemBase.h"

#include "Character/ISCharacter.h"
#include "Data/ISPlayerItemDataTable.h"
#include "Game/ISGameInstance.h"
#include "Game/ISGameplayMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AISItemBase::AISItemBase()
{
	PickUpCheckSphere = CreateDefaultSubobject<USphereComponent>("PickUpCheckSphere");
	PickUpCheckSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Block);  //设置游戏拾取检测通道默认为是
	SetRootComponent(PickUpCheckSphere);
	PrimaryActorTick.bCanEverTick = true;
	ItemsStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemsStaticMesh");
	ItemsStaticMesh->SetupAttachment(PickUpCheckSphere);
	bReplicates = true;  //设置为可复制
}

// Called when the game starts or when spawned
void AISItemBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AISItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AISItemBase::PickUpItemToInventory_Implementation(APawn* TargetPawn, AActor* TargetActor)
{
	IISItemInterface::PickUpItemToInventory_Implementation(TargetPawn, TargetActor);
	UISGameInstance* TargetGameInstance = Cast<UISGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!TargetGameInstance) return;
	const UDataTable*UserDT = TargetGameInstance->ItemDataTable;
	check(UserDT);
	AISItemBase*SourceItem = Cast<AISItemBase>(TargetActor);
	FName TargetItemID = SourceItem->ItemID;
	FItemInformation*UserInfo = UserDT->FindRow<FItemInformation>(TargetItemID,TEXT("name"));
	if (UserInfo)
	{
		AISCharacter*SourceCharacter = Cast<AISCharacter>(TargetPawn);
		if(!SourceCharacter) return;
		UISCharacterInventory*TargetInventory = SourceCharacter->CharacterInventory;
		UISHotBarInventory*TargetHotBar = SourceCharacter->CharacterHotBarInventory;
		if(!TargetInventory&&!TargetHotBar) return;
		//物品添加进物品栏
		if(TargetHotBar->CheckInventoryEmpty(*UserInfo))
		{
			TargetHotBar->ItemPickup.Broadcast(*UserInfo);
			TargetHotBar->InventoryUpdate.Broadcast();
			FString ItemNameToPrint = FString::Printf(TEXT("已拾取: %s"), *UserInfo->ItemName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *ItemNameToPrint);
			return;
		}
		//物品加进角色背包
		else if(TargetInventory->CheckInventoryEmpty(*UserInfo))
		{
			TargetInventory->ItemPickup.Broadcast(*UserInfo);
			TargetInventory->InventoryUpdate.Broadcast();
			FString ItemNameToPrint = FString::Printf(TEXT("已拾取: %s"), *UserInfo->ItemName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *ItemNameToPrint);
			return;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("背包已满！！"));
	}
}

void AISItemBase::UnUseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC)
{
	
}

void AISItemBase::UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC)
{
	
}
