// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Input/Reply.h"
#include "Components/SlateWrapperTypes.h"
#include "InputCoreTypes.h"

void UItemWidget::Refresh()
{
	check(InventoryComp);
	ReturnedRow = InventoryComp->ReturnItemInfo(ItemID);
	int X = ReturnedRow->SizeX;
	int Y = ReturnedRow->SizeY;
	X = X * TileSize;
	Y = Y * TileSize;
	Size.X = X;
	Size.Y = Y;
	SizeBox->SetWidthOverride(Size.X);
	SizeBox->SetHeightOverride(Size.Y);
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image->Slot);
	check(CanvasSlot);
	CanvasSlot->SetSize(Size);
	check(Image)
		Image->SetBrushFromMaterial(ReturnedRow->Icon);
	Image->SetBrushSize(Size);
}

void UItemWidget::SetVariables(UInventoryComponent* InvComp, int NewItemID, float NewTileSize, int NewIndex)
{
	InventoryComp = InvComp;
	ItemID = NewItemID;
	TileSize = NewTileSize;
	Index = NewIndex;
	CharcterRef = Cast<AFLoatingISlandRPGCharacter>(InventoryComp->GetOwner());
}

void UItemWidget::OnDestroy()
{
}

void UItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	check(Border);
	Border->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.4f));
}

void UItemWidget::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseLeave(MouseEvent);
	check(Border);
	Border->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.4f));
}

void UItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	OutOperation = Cast<UDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass()));
	OutOperation->DefaultDragVisual = this;
	OutOperation->Payload = this;
	check(InventoryComp);
	InventoryComp->ItemRemoved(ItemID, Index);
}

FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.GetEffectingButton() == EKeys::MiddleMouseButton)
	{
		InventoryComp->AddReserve(ItemID);
	}
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
	}
	FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return ReplyResult.NativeReply;
}

void UItemWidget::SetWidgetComponents(USizeBox* SizeBoxRef, UBorder* BorderRef, UImage* ImageRef)
{
	SizeBox = SizeBoxRef;
	Border = BorderRef;
	Image = ImageRef;
}

