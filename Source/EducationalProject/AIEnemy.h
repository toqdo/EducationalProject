// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerTeam.h"
#include "AIEnemy.generated.h"

class UAIPerceptionComponent;
class AAIEnemyController;
class AEPProjectile;
class UWidgetComponent;

UCLASS()
class EDUCATIONALPROJECT_API AAIEnemy : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIEnemy();

	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* PerceptionComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable)
	void Walk();

	UFUNCTION(BlueprintCallable)
	void Run();

	UFUNCTION(BlueprintCallable)
	bool OnFire();

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetMuzzleLocation();

	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void OnReload();

	UFUNCTION(BlueprintCallable)
	void OnReloadCycle();

	void SetHealth(float NewHealth);

	void AffectHealth(int32 Intensity);

	void AffectAmmo(int32 Intensity);

	float GetHealth() const { return Health; }

	float GetMaxHealth() const { return MaxHealth; }

	int32 GetAmmo() const { return Ammo; }

	int32 GetMaxAmmo() const { return MaxAmmo; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AActor* PerceivedActor;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DodgeMontage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float WalkSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WeaponEffectiveRange = 500.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsRunning = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasLineOfSight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetLostTime;

	UPROPERTY(BlueprintReadWrite)
	bool bIsReloading = false;

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

private:
	UFUNCTION()
	void OnActorPerceived(AActor* Actor, FAIStimulus Stimulus);

	float TargetLostElapsed = 0.f;

	float LostLocationElapsed = 0.f;

	float LostLocationUpdateTime = .5f;

	AAIEnemyController* AIController;

	FTimerHandle WidgetVisibilityTimerHandle;
};
