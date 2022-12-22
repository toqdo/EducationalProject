// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "../PlayerTeam.h"
#include "EPCharacter.generated.h"

class UCameraComponent;
class AEPProjectile;

UCLASS()
class EDUCATIONALPROJECT_API AEPCharacter : public ACharacter, public IGenericTeamAgentInterface
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

	float GetHealth() const;

	float GetMaxHealth() const;

	int32 GetAmmo() const;

	int32 GetMaxAmmo() const;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	const AActor* GetCurrentTarget();

	void SetHealth(float NewHealth);

	void AffectHealth(int32 Intensity);

	void AffectAmmo(int32 Intensity);

	UFUNCTION()
	void OnCapsuleHit(UPrimitiveComponent* HitComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* GunMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USceneComponent* MuzzleSocketComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEPProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Ammo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmmo = 10;

	AActor* CurrentTarget;

private:
	AActor* LastHitActor;

	FPlayerTeamID TeamID;
};
