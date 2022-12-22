// Copyright Epic Games, Inc. All Rights Reserved.

#include "EducationalProjectGameMode.h"
#include "EducationalProjectCharacter.h"
#include "Core/SpawEnemiesManager.h"
#include "UObject/ConstructorHelpers.h"

AEducationalProjectGameMode::AEducationalProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}


void AEducationalProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	EnemiesManager = GetWorld()->SpawnActor<ASpawnEnemiesManager>(SpawnEnemiesManagerClass);

	ItemsManager = GetWorld()->SpawnActor<AItemSpawnManager>(ItemSpawnManagerClass);
}
