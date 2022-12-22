// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthWidget.h"
#include "Core/EPEnemy.h"
#include "Components/ProgressBar.h"

void UEnemyHealthWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HealthBar->PercentDelegate.BindUFunction(this, TEXT("HealthBarPercent"));
}

float UEnemyHealthWidget::HealthBarPercent()
{
	UObject* Outer = GetOuter();
	AEPEnemy* Enemy;
	while (Outer)
	{
		Enemy = Cast<AEPEnemy>(Outer);
		if (Enemy)
		{
			return Enemy->GetHealth() / Enemy->GetMaxHealth();
		}

		Outer = Outer->GetOuter();
	}
	return 0.0f;
}