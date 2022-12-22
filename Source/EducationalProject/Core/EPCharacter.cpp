// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCharacter.h"
#include "Camera/CameraComponent.h"
#include "EPProjectile.h"
#include "Components/CapsuleComponent.h"
#include "EPEnemy.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEPCharacter::AEPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetRootComponent());

	GetMesh()->SetupAttachment(CameraComponent);

	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComponent"));
	GunMeshComponent->SetupAttachment(GetMesh());

	MuzzleSocketComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleSocketComponent"));
	MuzzleSocketComponent->SetupAttachment(GunMeshComponent);

}

// Called when the game starts or when spawned
void AEPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GunMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(this, &AEPCharacter::OnCapsuleHit);

	Health = MaxHealth;
	Ammo = MaxAmmo;
}

void AEPCharacter::OnCapsuleHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 5.0f, FColor::Red, true);

	if (OtherActor->GetClass()->IsChildOf(AEPEnemy::StaticClass()))
	{
		if (LastHitActor != OtherActor)
		{
			AEPEnemy* Enemy = Cast<AEPEnemy>(OtherActor);
			Enemy->Attack();

			FTimerHandle TimerHandle;

			GetWorldTimerManager().SetTimer(
				TimerHandle,
				FTimerDelegate::CreateLambda(
					[WeakCharacter = MakeWeakObjectPtr(this), OtherActor]()
					{
						if (WeakCharacter.IsValid())
						{
							UGameplayStatics::ApplyDamage(WeakCharacter.Get(), 10.f, OtherActor->GetOwner()->GetInstigatorController(), OtherActor, TSubclassOf<UDamageType>(UDamageType::StaticClass()));

							FVector Impulse = UKismetMathLibrary::GetDirectionUnitVector(OtherActor->GetActorLocation(), WeakCharacter->GetActorLocation());
							Impulse *= 1'000'000.f;
							WeakCharacter->GetCharacterMovement()->AddImpulse(Impulse);
						}
					}
				),
				1.f,
				false
			);
			UE_LOG(LogTemp, Warning, TEXT("Some warning message %f"), Health);
		}
		LastHitActor = OtherActor;
	}
}

// Called every frame
void AEPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEPCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Health > 0.f)
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

// Called to bind functionality to input
void AEPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AEPCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AEPCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AEPCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AEPCharacter::LookRight);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AEPCharacter::Fire);
	PlayerInputComponent->BindAction(TEXT("Trace"), EInputEvent::IE_Pressed, this, &AEPCharacter::Trace);
}

void AEPCharacter::Trace()
{
	FHitResult HitResult;
	
	FCollisionObjectQueryParams CollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByObjectType(
		HitResult, 
		CameraComponent->GetComponentLocation(),
		CameraComponent->GetComponentLocation() + GetControlRotation().Vector() * 10000.0f,
		CollisionObjectQueryParams,
		CollisionQueryParams
	);

	if (HitResult.Actor.IsValid() && HitResult.bBlockingHit)
	{
		CurrentTarget = HitResult.Actor.Get();
	}
	else
	{
		CurrentTarget = nullptr;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::Cyan, 
			FString::Printf(TEXT("%s"), HitResult.Actor.IsValid() ? *HitResult.Actor->GetName() : TEXT("none"))
		);
	}
}

void AEPCharacter::Fire()
{
	if (Ammo)
	{
		FVector SpawnLocation = MuzzleSocketComponent->GetComponentLocation();
		FRotator SpawnRotation = Controller->GetControlRotation();

		FTransform ActorTransform(SpawnRotation, SpawnLocation);

		AEPProjectile* Projectile = GetWorld()->SpawnActorDeferred<AEPProjectile>(ProjectileClass, ActorTransform, this,
			GetInstigator(), ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		Projectile->EPCharacter = this;
		Projectile->FinishSpawning(ActorTransform);

		--Ammo;
	}
}

void AEPCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		FVector Direction = GetActorForwardVector();
		AddMovementInput(Direction, Value);
	}
}

void AEPCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


void AEPCharacter::LookUp(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerPitchInput(Value * 45.0f * GetWorld()->GetDeltaSeconds());
	}
}


void AEPCharacter::LookRight(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value * 45.0f * GetWorld()->GetDeltaSeconds());
	}
}

const AActor* AEPCharacter::GetCurrentTarget()
{
	return CurrentTarget;
}

float AEPCharacter::GetHealth() const
{
	return Health;
}

float AEPCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

int32 AEPCharacter::GetAmmo() const
{
	return Ammo;
}

int32 AEPCharacter::GetMaxAmmo() const
{
	return MaxAmmo;
}

void AEPCharacter::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
}

void AEPCharacter::AffectHealth(int32 Intensity)
{
	SetHealth(Health + Intensity);
}

void AEPCharacter::AffectAmmo(int32 Intensity)
{
	Ammo = FMath::Clamp(Intensity + Ammo, 0, MaxAmmo);
}

FGenericTeamId AEPCharacter::GetGenericTeamId() const
{
	return TeamID.GetId();
}