// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstance.h"
#include <Kismet/GameplayStatics.h>

void URPGGameInstance::Init()
{
	//check if we have a save game, if not create one and populate its varaibles with defualt values
	Super::Init();
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0) == true)
	{
		FLRPGSaveGame = Cast<URPGSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		FLRPGSaveGame = Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
		FLRPGSaveGame->SetAudioVolume(1.0f, 1.0f, 1.0f);
		FLRPGSaveGame->SetToolTipIndex(0);
		FLRPGSaveGame->SetCurrentRecord(100.99);
	}
}

TArray<float> URPGGameInstance::LoadAudio()
{
	check(FLRPGSaveGame);
	return FLRPGSaveGame->GetAudioVolume();
}

void URPGGameInstance::SetAudio(float Master, float Music, float SFX)
{
	check(FLRPGSaveGame);
	FLRPGSaveGame->SetAudioVolume(Master, Music, SFX);
}

bool URPGGameInstance::HasGameBeenSaved()
{
	return UGameplayStatics::SaveGameToSlot(FLRPGSaveGame, SaveSlotName, 0);
}

void URPGGameInstance::SetToolTipIndex(int index)
{
	check(FLRPGSaveGame);
	FLRPGSaveGame->SetToolTipIndex(index);
}

int URPGGameInstance::LoadToolTipIndex()
{
	check(FLRPGSaveGame);
	return FLRPGSaveGame->GetToolTipIndex();
}

float URPGGameInstance::LoadCurrentRecord()
{
	check(FLRPGSaveGame);
	return FLRPGSaveGame->GetCurrentRecord();
}

void URPGGameInstance::SetCurrentRecord(float NewRecord)
{
	check(FLRPGSaveGame);
	FLRPGSaveGame->SetCurrentRecord(NewRecord);
}
