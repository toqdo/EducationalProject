// Fill out your copyright notice in the Description page of Project Settings.


#include "EPEnemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "../EP_AIController.h"
#include "EPProjectile.h"
#include "NavigationSystem.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEPEnemy::AEPEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(GetRootComponent());

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(CapsuleComponent);

	FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovementComponent"));
	FloatingPawnMovementComponent->SetUpdatedComponent(CapsuleComponent);
}

// Called when the game starts or when spawned
void AEPEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;

	CapsuleComponent->OnComponentHit.AddUniqueDynamic(this, &AEPEnemy::OnCapsuleHit);
	WidgetComponent->GetUserWidgetObject()->Rename(nullptr, WidgetComponent);

	WidgetComponent->SetVisibility(false);

	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &AEPEnemy::MoveToRandomPoint, 4.f, true);
}

void AEPEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Ensure the fuze timer is cleared by using the timer handle
	GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);

	// Alternatively you can clear ALL timers that belong to this (Actor) instance.
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AEPEnemy::OnCapsuleHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (Cast<AEPProjectile>(OtherActor) && Health > 0.f)
	{
		if (DamageAnimation)
		{
			SkeletalMeshComponent->PlayAnimation(DamageAnimation, false);
		}

		Health -= 20.f;
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

		if (FMath::IsNearlyZero(Health))
		{
			WidgetComponent->SetVisibility(false);

			GetWorldTimerManager().ClearTimer(WidgetVisibilityTimerHandle);

			SkeletalMeshComponent->PlayAnimation(DeathAnimation, false);

			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				AIController->StopMovement();
			}
			GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);


			FTimerHandle TimerHandle;

			GetWorldTimerManager().SetTimer(
				TimerHandle,
				FTimerDelegate::CreateLambda(
					[WeakEnemy = MakeWeakObjectPtr(this)]()
					{
						if (WeakEnemy.IsValid())
						{
							WeakEnemy->Destroy();
						}
					}
				),
				2.3f,
				false
			);

			IsDead = true;
		}
	}
}

// Called every frame
void AEPEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead)
	{
		if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController()) && IsValid(GetWorld()->GetFirstPlayerController()->GetPawn()))
		{
			FVector ActorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
			FVector DirectionToActor = ActorLocation - GetActorLocation();
			DirectionToActor.Z = 0.f;

			WidgetComponent->SetWorldRotation(DirectionToActor.Rotation());
		}
	}
}

void AEPEnemy::MoveToRandomPoint()
{
	FVector RandPoint;
	RandPoint = UNavigationSystemV1::GetCurrent(this)->GetRandomReachablePointInRadius(this, GetActorLocation(), 1000.0);
	RandPoint.Z = GetActorLocation().Z;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		FVector Direction = RandPoint - GetActorLocation();
		Direction.Z = 0.f;

		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), RandPoint));
		
		AIController->MoveToLocation(RandPoint);
	}
}

void AEPEnemy::Attack()
{
	if (AttackAnimation)
	{
		FVector ActorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ActorLocation));
		SkeletalMeshComponent->PlayAnimation(AttackAnimation, false);
	}
}