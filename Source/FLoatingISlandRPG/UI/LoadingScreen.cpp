// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreen.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/SoftObjectPath.h"
#include <Kismet/GameplayStatics.h>
#include "TimerManager.h"

void ULoadingScreen::StartLoading()
{
	GameInstance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	LoadPackageAsync(Level1Ptr.GetAssetName(), FLoadPackageAsyncDelegate::CreateUObject(this, &ULoadingScreen::FinishedLoading), TPrio, PKG_ContainsMap, int32(-1));
	fLevelName = FName(Level1Ptr.GetAssetName());
	GameInstance->HasGameBeenSaved();
}

void ULoadingScreen::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	fLoadingProgress = GetAsyncLoadPercentage(fLevelName);
	if (fLoadingProgress == -1)
	{
		fLoadingProgress = 1.0f;
	}
}

void ULoadingScreen::FinishedLoading(const FName& name, UPackage* Package, EAsyncLoadingResult::Type Arg)
{
	FTimerHandle UnusedHandle;
	uSaveFromGarbage = Package;
	fLoadingProgress = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ULoadingScreen::DelayedFinishedLoading, 2.0f);
}

void ULoadingScreen::DelayedFinishedLoading()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level1Ptr);
}

FString ULoadingScreen::GetToolTip()
{
	int ToolTipIndex = GameInstance->LoadToolTipIndex();
	if (ToolTipIndex >= NumberOfRowsInToolTIpTable)
	{
		ToolTipIndex = 0;
	}
	check(ToolTipTable);
	//currently set to cycle tool tips instead of picking random one
	//int ToolTipIndex;
	//ToolTipIndex = FMath::RandRange(0, 2);
	FName RowName = FName(*FString::FromInt(ToolTipIndex));
	const TCHAR* ContextString(TEXT("Context"));
	ReturnedRow = ToolTipTable->FindRow<FToolTipTable>(RowName, ContextString, true);
	ToolTipIndex += 1;
	GameInstance->SetToolTipIndex(ToolTipIndex);
	return ReturnedRow->ToolTip;
}
