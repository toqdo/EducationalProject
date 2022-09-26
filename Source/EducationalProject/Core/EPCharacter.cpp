// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCharacter.h"
#include "Camera/CameraComponent.h"
#include "EPProjectile.h"
#include "Blueprint/UserWidget.h"

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
}

// Called every frame
void AEPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	FVector SpawnLocation = MuzzleSocketComponent->GetComponentLocation();
	FRotator SpawnRotation = Controller->GetControlRotation();

	FTransform ActorTransform(SpawnRotation, SpawnLocation);

	AEPProjectile* Projectile = GetWorld()->SpawnActorDeferred<AEPProjectile>(ProjectileClass, ActorTransform, this, 
		GetInstigator(), ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
	Projectile->EPCharacter = this;
	Projectile->FinishSpawning(ActorTransform);
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