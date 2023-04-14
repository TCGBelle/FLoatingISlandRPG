// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "InputCoreTypes.h"
#include "EnhancedActionKeyMapping.h"
#include "OptionsMenu.generated.h"

/**
 * 
 */
UCLASS()
class FLOATINGISLANDRPG_API UOptionsMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	//functions
	UFUNCTION(BlueprintCallable)
		void ChangeResolution(FString InputLeft, FString InputRight);
	UFUNCTION(BlueprintCallable)
		void ChangeAllQualitys();
	UFUNCTION(BlueprintCallable)
		void SetAllProtectedQualityIntsByEngine();
	UFUNCTION(BlueprintCallable)
		void SetVolumeByClass(float Volume, USoundClass* ClassToChange, USoundMix* MixToChange);
	UFUNCTION(BlueprintCallable)
		float GetVolumeByClass(USoundClass* ClassToFetch);
	UFUNCTION(BlueprintCallable)
		void InitalizeKeys();
	UFUNCTION(BlueprintCallable)
		bool IsKeyUsed(FKey NewKey);
	UFUNCTION(BlueprintCallable)
		bool HaveSwapedKeyByName(FName NameofAction, FKey NewKey);
	UFUNCTION(BlueprintCallable)
		FKey GetKeyByName(FName NameofAction);
	//Properties
	UPROPERTY(BlueprintReadWrite)
		int ViewDistanceQuality;
	UPROPERTY(BlueprintReadWrite)
		int AntialiasingQuality;
	UPROPERTY(BlueprintReadWrite)
		int TextureQuality;
	UPROPERTY(BlueprintReadWrite)
		int ShadowQuality;
private:
	FString RightResolutionString;
	FString LeftResolutionString;
	UPROPERTY()
		UGameUserSettings* GameSettings;
	/** MappingContext */
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
		TArray<FKey> UsedKeys;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
		TArray<FEnhancedActionKeyMapping> AllMapings;
};
