// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleHook.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AGrappleHook::AGrappleHook()
{
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrappleHook::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->Velocity = Velocity;
	StartLocation = GetActorLocation();
	MaxDistance = 2500;
}

// Called every frame
void AGrappleHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrDistance = UKismetMathLibrary::Vector_Distance(StartLocation, GetActorLocation());
	if (CurrDistance >= MaxDistance)
	{
		Destroy();
	}
}

void AGrappleHook::Destroyed()
{
	DestroyedDelegate.Broadcast();
}

