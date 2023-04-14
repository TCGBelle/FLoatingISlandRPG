// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "../FLoatingISlandRPGCharacter.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class FLOATINGISLANDRPG_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Refresh();
	void SetVariables(UInventoryComponent* InvComp, int NewItemID, float NewTileSize, int NewIndex);
	void OnDestroy();
private:
	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void NativeOnMouseLeave(const FPointerEvent& MouseEvent);
	void NativeOnDragDetected(const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent);
protected:
	UFUNCTION(BlueprintCallable)
		void SetWidgetComponents(USizeBox* SizeBoxRef, UBorder* BorderRef, UImage* ImageRef);
public:
	int ItemID;
	float TileSize;
	int Index;
private:
	UPROPERTY()
		UInventoryComponent* InventoryComp;
	UPROPERTY()
		AFLoatingISlandRPGCharacter* CharcterRef;

	FItemTable* ReturnedRow;
	FVector2D Size;
	bool bBeingDragged;
	UPROPERTY(EditAnywhere, Category = "Components")
		USizeBox* SizeBox;
	UPROPERTY(EditAnywhere, Category = "Components")
		UBorder* Border;
	UPROPERTY(EditAnywhere, Category = "Components")
		UImage* Image;
};
