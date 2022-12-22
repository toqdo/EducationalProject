// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ItemSpawnManager.h"
#include "EducationalProjectGameMode.generated.h"

class ASpawnEnemiesManager;

UCLASS(minimalapi)
class AEducationalProjectGameMode : public AGameModeBase
{
	GENERATED_BODY() 

public:
	AEducationalProjectGameMode();

	ASpawnEnemiesManager* EnemiesManager;

	AItemSpawnManager* ItemsManager;

	UPROPERTY(EditAnywhere, Category="Classes")
	TSubclassOf<ASpawnEnemiesManager> SpawnEnemiesManagerClass;

	UPROPERTY(EditAnywhere, Category = "Classes")
	TSubclassOf<AItemSpawnManager> ItemSpawnManagerClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};



