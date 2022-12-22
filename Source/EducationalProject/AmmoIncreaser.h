// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableItem.h"
#include "AmmoIncreaser.generated.h"

/**
 * 
 */
UCLASS()
class EDUCATIONALPROJECT_API AAmmoIncreaser : public APickableItem
{
	GENERATED_BODY()

public:
    virtual void SetEffect_Implementation(AEPCharacter* OtherActor) override;
};
