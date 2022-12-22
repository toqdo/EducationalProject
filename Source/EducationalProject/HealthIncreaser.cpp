// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthIncreaser.h"
#include "Core/EPCharacter.h"

void AHealthIncreaser::SetEffect_Implementation(AEPCharacter* OtherActor)
{
    OtherActor->AffectHealth(Intensity);
}
