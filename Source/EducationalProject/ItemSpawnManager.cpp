// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnManager.h"
#include <Kismet/GameplayStatics.h>
#include "SpawnItemTargetPoint.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AItemSpawnManager::AItemSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	TArray <AActor*> FoundTargetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnItemTargetPoint::StaticClass(), FoundTargetPoints);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	int i = 0;
	for (auto TargetPoint : FoundTargetPoints)
	{
		APickableItem* Item = GetWorld()->SpawnActor<APickableItem>(ItemClasses[i], TargetPoint->GetActorTransform(), SpawnParams);
		Item->OnDestroyed.AddUniqueDynamic(this, &AItemSpawnManager::OnItemDestoyed);
		++i;
		if (i == ItemClasses.Num())
		{
			i = 0;
		}
	}
}

void AItemSpawnManager::SpawnItem(TSubclassOf<APickableItem> ItemClassToSpawn, FVector SpawnPoint)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APickableItem* Item = GetWorld()->SpawnActor<APickableItem>(ItemClassToSpawn, FTransform(SpawnPoint), SpawnParams);
	if (Item)
	{
		Item->OnDestroyed.AddUniqueDynamic(this, &AItemSpawnManager::OnItemDestoyed);
	}
}

void AItemSpawnManager::OnItemDestoyed(AActor* DestroyedActor)
{
	FTimerHandle CreationTimeHandler;
	GetWorldTimerManager().SetTimer(CreationTimeHandler,
		FTimerDelegate::CreateLambda(
			[ItemClassToSpawn = DestroyedActor->GetClass(), 
			SpawnPoint = DestroyedActor->GetActorLocation(), 
			WeakManager = MakeWeakObjectPtr(this)]()
			{
				WeakManager->SpawnItem(ItemClassToSpawn, SpawnPoint);
			}
		),
	    2.f, 
		false
	);
}