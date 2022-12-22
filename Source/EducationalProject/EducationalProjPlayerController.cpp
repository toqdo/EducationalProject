// Fill out your copyright notice in the Description page of Project Settings.


#include "../EducationalProject/EducationalProjPlayerController.h"
#include "Blueprint/UserWidget.h"

AEducationalProjPlayerController::AEducationalProjPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEducationalProjPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* CrosshairWidget = CreateWidget<UUserWidget>(this, CrosshairWidgetClass);
	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(0, 6, FColor::Black, TEXT("Crosshair Widget is nullptr"));
	}
	SetInputMode(FInputModeGameOnly());

}