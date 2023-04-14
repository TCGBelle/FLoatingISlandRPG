// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "InventoryGridWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class FLOATINGISLANDRPG_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetInvComp(UInventoryComponent* Inv);
	UFUNCTION(BlueprintCallable, Category = "Grid")
		void SetGrid();
	bool NativeOnDrop
	(
		const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation
	);
	UFUNCTION(BlueprintCallable)
		void SetInventoryRefrences(UInventoryGridWidget* Grid);

private:
	UPROPERTY()
		UInventoryComponent* InvComp;
	UPROPERTY(EditAnywhere, Category = "widgets")
		UInventoryGridWidget* GridRef;
};
