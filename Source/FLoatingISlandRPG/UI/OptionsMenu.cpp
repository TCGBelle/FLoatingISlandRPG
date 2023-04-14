// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenu.h"
#include <Kismet/KismetStringLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "InputMappingContext.h"

void UOptionsMenu::ChangeResolution(FString InputLeft, FString InputRight)
{
	// change the selection from the drop down menu to an Int and then set the resolution
	LeftResolutionString = InputLeft;
	RightResolutionString = InputRight;
	GameSettings = GEngine->GetGameUserSettings();
	check(GameSettings);
	FIntPoint TempIntPoint;
	TempIntPoint.X = FCString::Atoi(*LeftResolutionString);
	TempIntPoint.Y = FCString::Atoi(*RightResolutionString);
	GameSettings->SetScreenResolution(TempIntPoint);
	GameSettings->ApplySettings(false);
}

void UOptionsMenu::ChangeAllQualitys()
{
	//Change all the engine graphics quality settings at once, make sure Protected Quality Ints are Set First can all be set between 0-4
	GameSettings = GEngine->GetGameUserSettings();
	check(GameSettings);
	GameSettings->SetViewDistanceQuality(ViewDistanceQuality);
	GameSettings->SetAntiAliasingQuality(AntialiasingQuality);
	GameSettings->SetTextureQuality(TextureQuality);
	GameSettings->SetShadowQuality(ShadowQuality);
	GameSettings->SetShadingQuality(ShadowQuality);
	GameSettings->ApplySettings(false);
}

void UOptionsMenu::SetAllProtectedQualityIntsByEngine()
{
	//set all the protected quality ints to the values held by the engine currently
	GameSettings = GEngine->GetGameUserSettings();
	check(GameSettings);
	ViewDistanceQuality = GameSettings->GetViewDistanceQuality();
	AntialiasingQuality = GameSettings->GetAntiAliasingQuality();
	TextureQuality = GameSettings->GetTextureQuality();
	ShadowQuality = GameSettings->GetShadowQuality();
}

void UOptionsMenu::SetVolumeByClass(float Volume, USoundClass* ClassToChange, USoundMix* MixToChange)
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MixToChange, ClassToChange, Volume, 1.0f, 0.1f, true);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MixToChange);
}

float UOptionsMenu::GetVolumeByClass(USoundClass* ClassToFetch)
{
	float TempVolume = ClassToFetch->Properties.Volume;
	TempVolume = FMath::Clamp(TempVolume, 0.0f, 2.0f);
	return TempVolume;
}

void UOptionsMenu::InitalizeKeys()
{
	// populate arrays for all the mapings and keys
	AllMapings = DefaultMappingContext->GetMappings();

	UsedKeys.Empty();
	for (int index = 0; index < AllMapings.Num(); index++)
	{
		UsedKeys.Add(AllMapings[index].Key);
	}
}

bool UOptionsMenu::IsKeyUsed(FKey NewKey)
{
	for (int index = 0; index < UsedKeys.Num(); index++)
	{
		if (UsedKeys[index].GetFName() == NewKey.GetFName())
		{
			return true;
		}
	}
	return false;
}

bool UOptionsMenu::HaveSwapedKeyByName(FName NameofAction, FKey NewKey)
{
	//swap the key acosiated with an action by the actions name.
	for (int Index = 0; Index < AllMapings.Num(); Index++)
	{
		if (AllMapings[Index].PlayerMappableOptions.Name == NameofAction && AllMapings[Index].bIsPlayerMappable == true)
		{
			FEnhancedActionKeyMapping& TempMappingRef = DefaultMappingContext->GetMapping(Index);
			TempMappingRef.Key = NewKey;
			InitalizeKeys();
			return true;
		}
	}
	return false;
}

FKey UOptionsMenu::GetKeyByName(FName NameofAction)
{
	for (int Index = 0; Index < AllMapings.Num(); Index++)
	{
		if (AllMapings[Index].PlayerMappableOptions.Name == NameofAction && AllMapings[Index].bIsPlayerMappable == true)
		{
			return AllMapings[Index].Key;
		}
	}
	return FKey();
}
