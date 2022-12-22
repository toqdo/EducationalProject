// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class EDUCATIONALPROJECT_API UEnemyHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (BindWidget))
		UProgressBar* HealthBar;

	UFUNCTION()
		float HealthBarPercent();

public:
	virtual void NativeOnInitialized() override;
};
