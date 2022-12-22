// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/EPCharacter.h"
#include "PickableItem.generated.h"

UCLASS()
class EDUCATIONALPROJECT_API APickableItem : public AActor
{
	GENERATED_BODY()
	
public:
    // Sets default values for this actor's properties
    APickableItem();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //FORCEINLINE EItemType GetItemType() const { return Type; }

    FORCEINLINE float GetStunnTime() const { return StunnTime; }

    FORCEINLINE int32 GetIntensity() const { return Intensity; }

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UCapsuleComponent* CapsuleComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* StaticMeshComponent;

    UFUNCTION(BlueprintNativeEvent)
    void SetEffect(AEPCharacter* OtherActor);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //UPROPERTY(EditDefaultsOnly)
    //EItemType Type;

    UPROPERTY(EditAnywhere, Category = "Effect", meta = (ClampMin = 0, ClampMax = 100))
        int32 Intensity = 50;

    UPROPERTY(EditAnywhere, Category = "Effect", meta = (ClampMin = 0.f, ClampMax = 100.f))
        float StunnTime = 0.f;

    UPROPERTY(EditAnywhere, Category = "Rotation", meta = (ClampMin = 0.f, ClampMax = 100.f))
        float RotatingAngle;

private:
    UFUNCTION()
        void OnBeginOverlap(
            UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult& SweepResult);
};

