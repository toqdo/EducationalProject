// Fill out your copyright notice in the Description page of Project Settings.

#include "HUDWidget.h"
#include "Core/EPCharacter.h"

void UHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HealthBar->PercentDelegate.BindUFunction(this, TEXT("HealthBarPercent"));
	AmmoBar->PercentDelegate.BindUFunction(this, TEXT("AmmoBarPercent"));
}

float UHUDWidget::HealthBarPercent()
{
	AEPCharacter* Character = Cast<AEPCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Character)
	{
		return Character->GetHealth() / Character->GetMaxHealth();
	}
	return 0.0f;
}

float UHUDWidget::AmmoBarPercent()
{
	AEPCharacter* Character = Cast<AEPCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Character)
	{
		return (float)(Character->GetAmmo()) / Character->GetMaxAmmo();
	}
	return 0;
}