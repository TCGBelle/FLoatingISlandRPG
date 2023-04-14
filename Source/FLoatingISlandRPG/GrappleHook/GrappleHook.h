// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GrappleHook.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHookHit);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHookHit, AActor*, SelfActor, AActor*, OtherActor, FVector, NormalImpulse, const FHitResult&, Hit);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHookDestroyed);
UCLASS()
class FLOATINGISLANDRPG_API AGrappleHook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappleHook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadOnly)
		FVector Velocity;
	FHookDestroyed DestroyedDelegate;
private:
	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
		float MaxDistance;
	FVector StartLocation;
	float CurrDistance;
};
