// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableItem.h"
#include "ItemSpawnManager.generated.h"

UCLASS()
class EDUCATIONALPROJECT_API AItemSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APickableItem>> ItemClasses;

	UFUNCTION()
	void OnItemDestoyed(AActor* DestroyedActor);

	void SpawnItem(TSubclassOf<APickableItem> ItemClassToSpawn, FVector SpawnPoint);
};
