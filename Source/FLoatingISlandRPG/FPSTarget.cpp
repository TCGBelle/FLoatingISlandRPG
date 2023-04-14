// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSTarget.h"
#include "FLoatingISlandRPGProjectile.h"
#include "FLoatingISlandRPGGameMode.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AFPSTarget::AFPSTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSTarget::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &AFPSTarget::BeenHit);
}

// Called every frame
void AFPSTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSTarget::BeenHit(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& HitResult)
{
	if (Cast<AFLoatingISlandRPGProjectile>(Other))
	{
		AFLoatingISlandRPGGameMode* GameMode = Cast<AFLoatingISlandRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->TargetDestroyed();
		Destroy();
	}
}

