// Fill out your copyright notice in the Description page of Project Settings.


#include "EPProjectile.h"
#include "EPCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AEPProjectile::AEPProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());

	InitialLifeSpan = 10.0f;
}

// Called when the game starts or when spawned
void AEPProjectile::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentHit.AddDynamic(this, &AEPProjectile::OnCapsuleHit);
	
	if (EPCharacter && EPCharacter->GetCurrentTarget())
	{
		ProjectileMovementComponent->HomingTargetComponent = EPCharacter->GetCurrentTarget()->GetRootComponent();
		if (ProjectileMovementComponent->HomingTargetComponent.IsValid())
		{
			ProjectileMovementComponent->bIsHomingProjectile = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Initial Target Component is not set for %s"), *GetName());
	}
}

void AEPProjectile::OnCapsuleHit(UPrimitiveComponent* HitComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComp != HitComponent)
	{
		Destroy();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 5, FColor::Red,
				FString::Printf(TEXT("%s destroyed"), *GetName())
			);
		}
	}
}

// Called every frame
void AEPProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EPCharacter && EPCharacter->GetCurrentTarget()/* &&
		ProjectileMovementComponent->HomingTargetComponent != EPCharacter->GetCurrentTarget()->GetRootComponent()*/)
	{
		ProjectileMovementComponent->HomingTargetComponent = EPCharacter->GetCurrentTarget()->GetRootComponent();
		ProjectileMovementComponent->bIsHomingProjectile = true;
	}
}

