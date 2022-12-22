// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/EPProjectile.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAIEnemy::AAIEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AAIEnemy::BeginPlay()
{
	Super::BeginPlay();

//	GunMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	AIController = Cast<AAIEnemyController>(GetController());

	RunSpeed = GetCharacterMovement()->MaxWalkSpeed;
	WalkSpeed = RunSpeed / 4;
	
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIEnemy::OnActorPerceived);
	OnTakeAnyDamage.AddDynamic(this, &AAIEnemy::OnDamage);

	Health = MaxHealth;
	Ammo = MaxAmmo;

	GetMesh()->OnComponentHit.AddUniqueDynamic(this, &AAIEnemy::OnMeshHit);
	WidgetComponent->GetUserWidgetObject()->Rename(nullptr, WidgetComponent);
	WidgetComponent->SetVisibility(false);
}

void AAIEnemy::OnMeshHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<AEPProjectile>(OtherActor) && Health > 0.f)
	{
		WidgetComponent->SetVisibility(true);
		GetWorldTimerManager().SetTimer(
			WidgetVisibilityTimerHandle,
			FTimerDelegate::CreateLambda(
				[WeakEnemy = MakeWeakObjectPtr(this)]()
				{
					if (WeakEnemy.IsValid())
					{
						WeakEnemy->WidgetComponent->SetVisibility(false);;
					}
				}
			),
			1.f,
			false
		);
	}
	if (FMath::IsNearlyZero(Health))
	{
		WidgetComponent->SetVisibility(false);

		Destroy();
	}
}

// Called every frame
void AAIEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController()) && IsValid(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		FVector ActorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		FVector DirectionToActor = ActorLocation - GetActorLocation();
		DirectionToActor.Z = 0.f;

		WidgetComponent->SetWorldRotation(DirectionToActor.Rotation());
	}

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		GetActorLocation(),
		GetActorLocation(),
		500.0f,
		{ UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1) },
		false,
		{ this },
		EDrawDebugTrace::None,
		HitResult,
		true);

	AActor* Projectile = Cast<AEPProjectile>(HitResult.GetActor());
	if (Projectile && Projectile->GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		PlayAnimMontage(DodgeMontage);
	}

	if (bHasLineOfSight && (LostLocationElapsed += DeltaTime) > LostLocationUpdateTime)
	{
		LostLocationElapsed = 0.f;
		AIController->GetBlackboardComponent()->SetValueAsVector(AIController->GetLastKnownLocationKey(), PerceivedActor->GetActorLocation());
	}

	if (!bHasLineOfSight && PerceivedActor && (TargetLostElapsed += DeltaTime) > TargetLostTime)
	{
		PerceivedActor = nullptr;
		TargetLostElapsed = 0.f;
		AIController->GetBlackboardComponent()->SetValueAsObject(AIController->GetDetectedEnemyKey(), PerceivedActor);
	}
}

// Called to bind functionality to input
void AAIEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AAIEnemy::OnActorPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	bHasLineOfSight = Stimulus.WasSuccessfullySensed();

	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("%d"), Stimulus.WasSuccessfullySensed()));

	AIController->GetBlackboardComponent()->SetValueAsBool(AIController->GetHasLineOfSightKey(), bHasLineOfSight);

	if (Stimulus.WasSuccessfullySensed())
	{
		PerceivedActor = Actor;
		TargetLostElapsed = 0.f;
		AIController->GetBlackboardComponent()->SetValueAsObject(AIController->GetDetectedEnemyKey(), PerceivedActor);
		AIController->GetBlackboardComponent()->SetValueAsVector(AIController->GetLastKnownLocationKey(), PerceivedActor->GetActorLocation());
	}
}

ETeamAttitude::Type AAIEnemy::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	return OtherTeamAgent && OtherTeamAgent->GetGenericTeamId().GetId() == PlayersTeam ?
		ETeamAttitude::Hostile : ETeamAttitude::Neutral;
}

void AAIEnemy::Run()
{
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AAIEnemy::Walk()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

bool AAIEnemy::OnFire()
{

	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("ammo %d fire"), Ammo));
	if (Ammo > 0)
	{
		FVector SpawnLocation = GetMuzzleLocation();
		FRotator SpawnRotation = Controller->GetControlRotation();

		FTransform ActorTransform(SpawnRotation, SpawnLocation);

		AEPProjectile* Projectile = GetWorld()->SpawnActorDeferred<AEPProjectile>(ProjectileClass, ActorTransform, this,
			nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->FinishSpawning(ActorTransform);

		--Ammo;
		return true;
	}

	return false;
}

void AAIEnemy::OnDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Cast<AEPProjectile>(DamageCauser))
	{
		if (Damage <= 0) return;

		Health = FMath::Clamp((float)Health - Damage, 0.0f, (float)MaxHealth);

		if (Health <= 0)
		{
			//Death();
			return;
		}

		if (!bHasLineOfSight)
		{
			PerceivedActor = GetWorld()->GetFirstPlayerController()->GetPawn();
			AIController->GetBlackboardComponent()->SetValueAsObject(AIController->GetDetectedEnemyKey(), PerceivedActor);
			AIController->GetBlackboardComponent()->SetValueAsVector(AIController->GetLastKnownLocationKey(), PerceivedActor->GetActorLocation());

			bHasLineOfSight = true;
			AIController->GetBlackboardComponent()->SetValueAsBool(AIController->GetHasLineOfSightKey(), bHasLineOfSight);

			FRotator FindedLook = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation());
			FindedLook.Pitch = 0.0f;
			SetActorRotation(FindedLook);
		}

	}
}

void AAIEnemy::OnReload()
{
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("ammo reload %d reload"), Ammo));

	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("ammo reload is reloading %d"), bIsReloading));
	if (Ammo < MaxAmmo)
	{
		bIsReloading = true;
		AIController->GetBlackboardComponent()->SetValueAsBool(AIController->GetIsReloadingKey(), bIsReloading);
	}
}

void AAIEnemy::OnReloadCycle()
{
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("ammo loop %d loop"), Ammo));
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("ammo loop is reloading %d"), bIsReloading));
	if (Ammo < MaxAmmo && ++Ammo == MaxAmmo)
	{
		bIsReloading = false;
		AIController->GetBlackboardComponent()->SetValueAsBool(AIController->GetIsReloadingKey(), bIsReloading);
	}
}