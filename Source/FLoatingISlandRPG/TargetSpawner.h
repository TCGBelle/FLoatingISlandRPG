// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetSpawner.generated.h"

UCLASS()
class FLOATINGISLANDRPG_API ATargetSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetSpawner();
	UFUNCTION(BlueprintCallable)
		void SpawnTarget();
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> TargetClass;
	UPROPERTY()
		AActor* Target;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
