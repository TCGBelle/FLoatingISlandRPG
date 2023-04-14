// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGSaveGame.h"

void URPGSaveGame::SetAudioVolume(float Master, float Music, float SFX)
{
	MasterVolume = Master;
	MusicVolume = Music;
	SFXVolume = SFX;
}

TArray<float> URPGSaveGame::GetAudioVolume()
{
	// 0 = master, 1 = music, 2 = SFX
	TArray<float> ReturnArray;
	ReturnArray.Add(MasterVolume);
	ReturnArray.Add(MusicVolume);
	ReturnArray.Add(SFXVolume);
	return ReturnArray;
}

void URPGSaveGame::SetToolTipIndex(int CurrIndex)
{
	ToolTipIndex = CurrIndex;
}

int URPGSaveGame::GetToolTipIndex()
{
	return ToolTipIndex;
}

void URPGSaveGame::SetCurrentRecord(float NewRecord)
{
	CurrentRecord = NewRecord;
}

float URPGSaveGame::GetCurrentRecord()
{
	return CurrentRecord;
}
