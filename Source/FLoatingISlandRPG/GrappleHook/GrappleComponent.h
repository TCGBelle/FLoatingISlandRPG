// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleHook.h"
#include "GrappleCable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GrappleComponent.generated.h"

UENUM(BlueprintType)
enum class EGrappleState : uint8
{
	ReadyToFire = 0 UMETA(DisplayName = "ReadyToFire"),
	Firing = 1 UMETA(DisplayName = "Firing"),
	AttatchedToTarget = 2 UMETA(DisplayName = "AttatchedToTarget")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLOATINGISLANDRPG_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrappleComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UFUNCTION()
		bool IsInUse() const;
	UFUNCTION()
		void FireGrappleHook(FVector TargetLocation, FVector LocalOffSet);
	UFUNCTION()
		void CancelGrappleHook();
private:
	UFUNCTION()
		void OnHookHit(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& HitResult);
	UFUNCTION()
		void OnHookDestroyed();
	FVector ToGrappleHook();
	FVector ToGrappleHook2D();
	FVector GetCableStartLocation(const FVector LocalOffSet);

	UPROPERTY(EditAnywhere)
	EGrappleState GrappleState;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AGrappleHook> GrappleHookClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AGrappleCable> GrappleCableClass;

	UPROPERTY()
		AGrappleHook* CurrentHook;
	UPROPERTY()
		AGrappleCable* CurrentCable;
	UPROPERTY()
		ACharacter* PersistentOwner;
	UPROPERTY()
		UCharacterMovementComponent* PersistentOwnerMovmentComponent;
	FVector InitalHookDirection;
};
