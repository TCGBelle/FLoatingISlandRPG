// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Ui/OptionsMenu.h"
#include "RPGSaveGame.h"
#include "RPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FLOATINGISLANDRPG_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	void Init() override;
public:
	UFUNCTION(BlueprintCallable)
		TArray<float> LoadAudio();
	UFUNCTION(BlueprintCallable)
		void SetAudio(float Master, float Music, float SFX);
	UFUNCTION(BlueprintCallable)
		bool HasGameBeenSaved();
	UFUNCTION(BlueprintCallable)
		void SetToolTipIndex(int index);
	UFUNCTION(BlueprintCallable)
		int LoadToolTipIndex();
	UFUNCTION(BlueprintCallable)
		float LoadCurrentRecord();
	UFUNCTION(BlueprintCallable)
		void SetCurrentRecord(float NewRecord);
private:
	UPROPERTY(EditAnywhere)
		FString SaveSlotName;
	UPROPERTY()
		URPGSaveGame* FLRPGSaveGame;
	UPROPERTY(EditAnywhere)
		TSubclassOf<USaveGame> SaveGameClass;
};
