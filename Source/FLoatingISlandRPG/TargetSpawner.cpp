// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetSpawner.h"
#include "FPSTarget.h"

// Sets default values
ATargetSpawner::ATargetSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATargetSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATargetSpawner::SpawnTarget()
{
	FActorSpawnParameters SapwnParams;
	SapwnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Target = GetWorld()->SpawnActor<AFPSTarget>(TargetClass, GetActorLocation(), GetActorRotation(), SapwnParams);
}

