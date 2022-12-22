// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EP_AIController.generated.h"

/**
 * 
 */
UCLASS()
class EDUCATIONALPROJECT_API AEP_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AEP_AIController();

	virtual void BeginPlay() override;
};
