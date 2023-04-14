// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "LoadingScreen.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	MainMenuClass = nullptr;
	OptionsMenuClass = nullptr;
	LoadingScreenClass = nullptr;
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	PlayerOne = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	MainMenu = CreateWidget<UMainMenu>(PlayerOne, MainMenuClass);
	OptionsMenu = CreateWidget<UOptionsMenu>(PlayerOne, OptionsMenuClass);
	LoadingScreen = CreateWidget<ULoadingScreen>(PlayerOne, LoadingScreenClass);
	CurrentUi = MainMenu;
	CurrentUi->AddToViewport();
	PlayerOne->SetShowMouseCursor(true);
}

void AMainMenuGameMode::OpenPassedUi(UUserWidget* Refrence)
{
	CurrentUi->RemoveFromParent();
	CurrentUi = Refrence;
	CurrentUi->AddToViewport();
}
