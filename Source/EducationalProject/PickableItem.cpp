// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"
#include "Components/CapsuleComponent.h"

APickableItem::APickableItem()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->SetupAttachment(GetRootComponent());

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(CapsuleComponent);
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
    Super::BeginPlay();

    CapsuleComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &APickableItem::OnBeginOverlap);
}

// Called every frame
void APickableItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator{ 0.f, RotatingAngle * DeltaTime, 0.f });
}

void APickableItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AEPCharacter* ProjectCharacter = Cast<AEPCharacter>(OtherActor))
    {
        SetEffect(ProjectCharacter);
        Destroy();
    }
}

void APickableItem::SetEffect_Implementation(AEPCharacter* OtherActor)
{
}
