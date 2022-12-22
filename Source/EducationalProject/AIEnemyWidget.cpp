// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyWidget.h"
#include "AIEnemy.h"
#include "Components/ProgressBar.h"

void UAIEnemyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HealthBar->PercentDelegate.BindUFunction(this, TEXT("HealthBarPercent"));
}

float UAIEnemyWidget::HealthBarPercent()
{
	UObject* Outer = GetOuter();
	AAIEnemy* Enemy;
	while (Outer)
	{
		Enemy = Cast<AAIEnemy>(Outer);
		if (Enemy)
		{
			return Enemy->GetHealth() / Enemy->GetMaxHealth();
		}

		Outer = Outer->GetOuter();
	}
	return 0.0f;
}
