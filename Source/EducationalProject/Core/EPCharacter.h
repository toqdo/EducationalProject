// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EPCharacter.generated.h"

class UCameraComponent;
class AEPProjectile;

UCLASS()
class EDUCATIONALPROJECT_API AEPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEPCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void LookUp(float Value);

	void LookRight(float Value);

	void Fire();

	void Trace();

	const AActor* GetCurrentTarget();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* GunMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USceneComponent* MuzzleSocketComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEPProjectile> ProjectileClass;

	AActor* CurrentTarget;
};
