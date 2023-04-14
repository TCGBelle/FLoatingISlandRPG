// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "Engine/EngineTypes.h"
#include "CableComponent.h"
#include "../FLoatingISlandRPGCharacter.h"

// Sets default values for this component's properties
UGrappleComponent::UGrappleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GrappleState == EGrappleState::AttatchedToTarget)
	{
		check(PersistentOwnerMovmentComponent)
		PersistentOwnerMovmentComponent->AddForce(ToGrappleHook() * 100000);
		UKismetMathLibrary::Vector_Normalize(InitalHookDirection, 0.001f);
		float Distance = UKismetMathLibrary::Vector_Distance(CurrentHook->GetActorLocation(), PersistentOwner->GetActorLocation());
		if (Distance < 250)
		{
			CurrentHook->Destroy();
			PersistentOwnerMovmentComponent->Velocity += FVector(0.0f, 0.0f, 100.0f);
		}
		else
		{
			float Dot = UKismetMathLibrary::Dot_VectorVector(InitalHookDirection, ToGrappleHook2D());
			if (Dot < 0.0f)
			{
				CurrentHook->Destroy();
			}
		}
	}
	// ...
}

bool UGrappleComponent::IsInUse() const
{
	switch(GrappleState)
	{
	case EGrappleState::ReadyToFire:
		return false;
		break;
	case EGrappleState::Firing:
		return true;
		break;
	case EGrappleState::AttatchedToTarget:
		return true;
		break;
	default:
		return true;
		break;
	}
}

void UGrappleComponent::FireGrappleHook(FVector TargetLocation, FVector LocalOffSet)
{
	if (!IsInUse())
	{
		check(GrappleHookClass);
		GrappleState = EGrappleState::Firing;
		FVector WorldStart = GetCableStartLocation(LocalOffSet);
		FVector FireDirection = TargetLocation - WorldStart;
		UKismetMathLibrary::Vector_Normalize(FireDirection, 0.001f);
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		UWorld* WorldRef = GetWorld();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetCableStartLocation(LocalOffSet));
		PersistentOwner = Cast<ACharacter>(GetOwner());
		CurrentHook = WorldRef->SpawnActorDeferred<AGrappleHook>(GrappleHookClass, SpawnTransform);
		CurrentHook->Velocity = FireDirection * 5000;
		UGameplayStatics::FinishSpawningActor(CurrentHook, SpawnTransform);
		//add delegate on ht to hook
		CurrentHook->OnActorHit.AddDynamic(this, &UGrappleComponent::OnHookHit);
		//add delegate on destroy to hook
		CurrentHook->DestroyedDelegate.AddDynamic(this, &UGrappleComponent::OnHookDestroyed);
		CurrentCable = WorldRef->SpawnActor<AGrappleCable>(GrappleCableClass, GetCableStartLocation(LocalOffSet), UKismetMathLibrary::MakeRotFromX(FireDirection),ActorSpawnParams);
		FAttachmentTransformRules AttatchRules{ EAttachmentRule::KeepWorld, true};
		CurrentCable->AttachToActor(PersistentOwner, AttatchRules, TEXT("None"));
		CurrentCable->CableComponent->SetAttachEndTo(CurrentHook, TEXT("None"), TEXT("None"));
		CurrentCable->CableComponent->EndLocation = FVector(0.0f, 0.0f, 0.0f);
	}
}

void UGrappleComponent::CancelGrappleHook()
{
	check(CurrentHook);
	CurrentHook->Destroy();
}

void UGrappleComponent::OnHookHit(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& HitResult)
{
	GrappleState = EGrappleState::AttatchedToTarget;
	check(PersistentOwner);
	PersistentOwnerMovmentComponent = PersistentOwner->GetCharacterMovement();
	PersistentOwnerMovmentComponent->GroundFriction = 0.0f;
	PersistentOwnerMovmentComponent->GravityScale = 0.0f;
	PersistentOwnerMovmentComponent->AirControl = 0.2;
	PersistentOwnerMovmentComponent->Velocity = (ToGrappleHook()*1200);
	InitalHookDirection = ToGrappleHook2D();
}

void UGrappleComponent::OnHookDestroyed()
{
	CurrentHook = nullptr;
	GrappleState = EGrappleState::ReadyToFire;
	check(CurrentCable);
	CurrentCable->Destroy();
	CurrentCable = nullptr;
	PersistentOwnerMovmentComponent = PersistentOwner->GetCharacterMovement();
	PersistentOwnerMovmentComponent->GroundFriction = 1.0f; //reset to defualts
	PersistentOwnerMovmentComponent->GravityScale = 1.0f;
	PersistentOwnerMovmentComponent->AirControl = 0.1;
}

FVector UGrappleComponent::ToGrappleHook()
{
	FVector HookLocation = CurrentHook->GetActorLocation();
	FVector OwnersLocation = PersistentOwner->GetActorLocation();
	FVector GrappleDirection = (HookLocation - OwnersLocation);
	UKismetMathLibrary::Vector_Normalize(GrappleDirection, 0.001f);
	return GrappleDirection;
}

FVector UGrappleComponent::ToGrappleHook2D()
{
	FVector Direction2D = ToGrappleHook();
	Direction2D.Z = 0.0f;
	UKismetMathLibrary::Vector_Normalize(Direction2D, 0.001f);
	return Direction2D;
}

FVector UGrappleComponent::GetCableStartLocation(const FVector LocalOffSet)
{
	PersistentOwner = Cast<ACharacter>(GetOwner());
	FTransform OwnersTransform = PersistentOwner->GetActorTransform();
	FVector WorldStart = (PersistentOwner->GetActorLocation() + UKismetMathLibrary::TransformDirection(OwnersTransform, LocalOffSet));
	return WorldStart;
}

