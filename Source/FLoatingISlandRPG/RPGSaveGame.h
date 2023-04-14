// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RPGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FLOATINGISLANDRPG_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	//Audio Functions
	UFUNCTION(BlueprintCallable)
		void SetAudioVolume(float Master, float Music, float SFX);
	UFUNCTION(BlueprintCallable)
		TArray<float> GetAudioVolume();
	UFUNCTION(BlueprintCallable)
		void SetToolTipIndex(int CurrIndex);
	UFUNCTION(BlueprintCallable)
		int GetToolTipIndex();
	UFUNCTION(BlueprintCallable)
		void SetCurrentRecord(float NewRecord);
	UFUNCTION(BlueprintCallable)
		float GetCurrentRecord();
private:
	//Audio Properties
	UPROPERTY()
		float MasterVolume;
	UPROPERTY()
		float MusicVolume;
	UPROPERTY()
		float SFXVolume;
	//tool tip index (for cycling tool tops like jamie wanted)
	UPROPERTY()
		int ToolTipIndex;
	UPROPERTY()
		float CurrentRecord;
};
