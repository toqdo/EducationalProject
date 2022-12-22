// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoIncreaser.h"
#include "Core/EPCharacter.h"

void AAmmoIncreaser::SetEffect_Implementation(AEPCharacter* OtherActor)
{
    OtherActor->AffectAmmo(Intensity);
}