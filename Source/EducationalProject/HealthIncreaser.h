// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableItem.h"
#include "HealthIncreaser.generated.h"

/**
 * 
 */
UCLASS()
class EDUCATIONALPROJECT_API AHealthIncreaser : public APickableItem
{
	GENERATED_BODY()
	
public:
	virtual void SetEffect_Implementation(class AEPCharacter* OtherActor) override;
};
