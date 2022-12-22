// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDUCATIONALPROJECT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta=(BindWidget))
	UProgressBar* HealthBar;

	UFUNCTION()
	float HealthBarPercent();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (BindWidget))
	UProgressBar* AmmoBar;

	UFUNCTION()
	float AmmoBarPercent();

public:
	virtual void NativeOnInitialized() override;
};
