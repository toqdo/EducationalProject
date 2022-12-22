// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EPEnemy.generated.h"

class UCapsuleComponent;
class UWidgetComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPEnemy : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEPEnemy();

	UFUNCTION()
	void OnCapsuleHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UWidgetComponent* WidgetComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UFloatingPawnMovement* FloatingPawnMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* DeathAnimation;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* AttackAnimation;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* DamageAnimation;

	UPROPERTY(EditDefaultsOnly)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetHealth() { return Health; }

	float GetMaxHealth() { return MaxHealth; }

	void Attack();

private:
	bool IsDead = false;

	FTimerHandle WidgetVisibilityTimerHandle;

	void MoveToRandomPoint();

	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	FTimerHandle MoveTimerHandle;
};
