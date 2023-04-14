// Copyright Epic Games, Inc. All Rights Reserved.

#include "FLoatingISlandRPGGameMode.h"
#include "FLoatingISlandRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/GameOverScreenWidget.h"
#include <Kismet/GameplayStatics.h>
#include "TargetSpawner.h"

AFLoatingISlandRPGGameMode::AFLoatingISlandRPGGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	GameOverScreenClass = nullptr;

	TotalTargets = 20;
	TargetsLeft = TotalTargets;
}

void AFLoatingISlandRPGGameMode::BeginPlay()
{
	PlayerOne = GEngine->GetFirstLocalPlayerController(GetWorld());
	GameOverScreen = CreateWidget<UGameOverScreenWidget>(PlayerOne, GameOverScreenClass);
	QuestHud = CreateWidget<UUserWidget>(PlayerOne, QuestHudClass);
	DialougeHud = CreateWidget<UUserWidget>(PlayerOne, DialougeHudClass);
	DialougeHud->AddToViewport();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnerClass, FoundSpawners);
	TotalTargets = FoundSpawners.Num();
}

void AFLoatingISlandRPGGameMode::TargetDestroyed()
{
	TargetsLeft--;
	if (TargetsLeft == 0)
	{
		GameOver();
	}
}

void AFLoatingISlandRPGGameMode::StartQuest()
{
	//Spawn Quest Hud
	QuestHud->AddToViewport();
	//delay by 3 seconds
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AFLoatingISlandRPGGameMode::ActivateSpawners, 3.0f);
}

int AFLoatingISlandRPGGameMode::GetTotalTargets()
{
	return TotalTargets;
}

int AFLoatingISlandRPGGameMode::GetTargetsLeft()
{
	return TargetsLeft;
}

void AFLoatingISlandRPGGameMode::SetCurrentRunsTime(float NewTime)
{
	CurrentRunsTime = NewTime;
}

float AFLoatingISlandRPGGameMode::GetCurrentRunsTime()
{
	return CurrentRunsTime;
}

void AFLoatingISlandRPGGameMode::ActivateSpawners()
{
	ATargetSpawner* CurrentSpawner;
	for (int incramenter = 0; incramenter < FoundSpawners.Num(); incramenter++)
	{
		CurrentSpawner = Cast<ATargetSpawner>(FoundSpawners[incramenter]);
		CurrentSpawner->SpawnTarget();
	}
	TargetsLeft = FoundSpawners.Num(); //this way number of targtets is variable for easier or harder gameplay
}

void AFLoatingISlandRPGGameMode::GameOver()
{
	PlayerOne->SetPause(true);
	PlayerOne->SetShowMouseCursor(true);
	QuestHud->RemoveFromParent();
	GameOverScreen->AddToViewport();
}
