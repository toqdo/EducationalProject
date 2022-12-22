// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class EDUCATIONALPROJECT_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:

	AAIEnemyController();

	float GetPatrolRadius() const { return PatrolRadius; }

	const FName& GetPatrolTargetKey() const { return PatrolTargetKey; }

	const FName& GetHasLineOfSightKey() const { return HasLineOfSightKey; }
	
	const FName& GetDetectedEnemyKey() const { return DetectedEnemyKey; }
	
	const FName& GetLastKnownLocationKey() const { return LastKnownLocationKey; }
	
	const FName& GetIsReloadingKey() const { return IsReloadingKey; }

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PatrolRadius = 1000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName PatrolTargetKey = "PatrolTarget";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName HasLineOfSightKey = "HasLineOfSight";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName DetectedEnemyKey = "DetectedEnemy";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName LastKnownLocationKey = "LastKnownLocation";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName IsReloadingKey = "IsReloading";

private:
};
