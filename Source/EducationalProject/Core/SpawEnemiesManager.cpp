// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawEnemiesManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SpawnEnemyTargetPoint.h"

// Sets default values
ASpawnEnemiesManager::ASpawnEnemiesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpawnEnemiesManager::BeginPlay()
{
	Super::BeginPlay();

	TArray < AActor*> FoundTargetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnEnemyTargetPoint::StaticClass(), FoundTargetPoints);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (AActor* FoundTargetPoint : FoundTargetPoints)
	{
		AEPEnemy* Enemy = GetWorld()->SpawnActor<AEPEnemy>(BaseEnemyBP, FoundTargetPoint->GetActorTransform(), SpawnParams);
		Enemy->OnDestroyed.AddUniqueDynamic(this, &ASpawnEnemiesManager::OnItemDestoyed);
	}
}


void ASpawnEnemiesManager::SpawnItem(TSubclassOf<AEPEnemy> ItemClassToSpawn, FVector SpawnPoint)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AEPEnemy* Item = GetWorld()->SpawnActor<AEPEnemy>(ItemClassToSpawn, FTransform(SpawnPoint), SpawnParams);
	Item->OnDestroyed.AddUniqueDynamic(this, &ASpawnEnemiesManager::OnItemDestoyed);
}

void ASpawnEnemiesManager::OnItemDestoyed(AActor* DestroyedActor)
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
		5.f,
		false
	);
}