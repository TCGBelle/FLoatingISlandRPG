// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class FLOATINGISLANDRPG_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
//Functions
public:
	AMainMenuGameMode();
public:
	virtual void BeginPlay();
	UFUNCTION(BlueprintCallable)
		void OpenPassedUi(UUserWidget* Refrence);
//Properties
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UMainMenu> MainMenuClass;
	UPROPERTY(BlueprintReadOnly)
		class UMainMenu* MainMenu;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UOptionsMenu> OptionsMenuClass;
	UPROPERTY(BlueprintReadOnly)
		class UOptionsMenu* OptionsMenu;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ULoadingScreen> LoadingScreenClass;
	UPROPERTY(BlueprintReadOnly)
		class ULoadingScreen* LoadingScreen;
private:
	UPROPERTY()
		class UUserWidget* CurrentUi;
	UPROPERTY()
		APlayerController* PlayerOne;
	FString LevelOne;
};
