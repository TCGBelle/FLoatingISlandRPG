// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Engine/DataTable.h"
#include "../RPGGameInstance.h"
#include "LoadingScreen.generated.h"

USTRUCT(BlueprintType)
struct FToolTipTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ToolTip;
};

UCLASS()
class FLOATINGISLANDRPG_API ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable)
		void StartLoading();
	void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	void FinishedLoading(const FName& name, UPackage* Package, EAsyncLoadingResult::Type Arg);
	void DelayedFinishedLoading();
	UFUNCTION(BlueprintCallable)
		FString GetToolTip();
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UDataTable* ToolTipTable;
	UPROPERTY(BlueprintReadOnly)
		float fLoadingProgress = 0.0f;
private:
	UPROPERTY(EditAnywhere, Category = LevelPointers)
		TSoftObjectPtr<UWorld> Level1Ptr;
	UPROPERTY(EditAnywhere)
		int NumberOfRowsInToolTIpTable; //needs set as there is no way to get the number of rows from the tool tip table
	UPROPERTY()
		UPackage* uSaveFromGarbage;
	UPROPERTY()
		URPGGameInstance* GameInstance;
	FToolTipTable* ReturnedRow;
	TAsyncLoadPriority TPrio;
	FName fLevelName;
};
