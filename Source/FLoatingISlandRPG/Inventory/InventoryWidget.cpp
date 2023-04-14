// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ItemWidget.h"

void UInventoryWidget::SetInvComp(UInventoryComponent* Inv)
{
	InvComp = Inv;
}

void UInventoryWidget::SetGrid()
{
	check(GridRef);
	GridRef->InitalizeGrid(InvComp, 75.0f);
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UItemWidget* DropedItem = Cast<UItemWidget>(InOperation->Payload);
	check(DropedItem);
	InvComp->SpawnItem(DropedItem->ItemID);
	DropedItem->RemoveFromParent();
	return true;
}

void UInventoryWidget::SetInventoryRefrences(UInventoryGridWidget* Grid)
{
	GridRef = Grid;
}

