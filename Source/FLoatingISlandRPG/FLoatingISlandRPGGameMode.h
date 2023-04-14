// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FLoatingISlandRPGGameMode.generated.h"

UCLASS(minimalapi)
class AFLoatingISlandRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFLoatingISlandRPGGameMode();
	virtual void BeginPlay();
	void TargetDestroyed();
	UFUNCTION(BlueprintCallable)
		void StartQuest();
	UFUNCTION(BlueprintCallable)
		int GetTotalTargets();
	UFUNCTION(BlueprintCallable)
		int GetTargetsLeft();
	UFUNCTION(BlueprintCallable)
		void SetCurrentRunsTime(float NewTime);
	UFUNCTION(BlueprintCallable)
		float GetCurrentRunsTime();
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UGameOverScreenWidget> GameOverScreenClass;
	UPROPERTY()
		UGameOverScreenWidget* GameOverScreen;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> QuestHudClass;
	UPROPERTY()
		UUserWidget* QuestHud;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> DialougeHudClass;
	UPROPERTY(BlueprintReadWrite)
		UUserWidget* DialougeHud;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> SpawnerClass;
	UPROPERTY()
		TArray<AActor*>FoundSpawners;
private:
	void GameOver();
	void ActivateSpawners();
	UPROPERTY()
		APlayerController* PlayerOne;

	int TotalTargets;
	int TargetsLeft;
	float CurrentRunsTime;
};



