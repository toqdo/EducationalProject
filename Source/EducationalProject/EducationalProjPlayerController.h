// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EducationalProjPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EDUCATIONALPROJECT_API AEducationalProjPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEducationalProjPlayerController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CrosshairWidgetClass;
};
