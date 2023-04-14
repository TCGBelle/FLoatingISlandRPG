// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGridWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/LineBatchComponent.h"
#include "ItemWidget.h"
#include <math.h>

void UInventoryGridWidget::InitalizeGrid(UInventoryComponent* InvComp, float Tilefloat)
{
	TileSize = Tilefloat;
	float X;
	float Y;
	InvComponent = InvComp;
	X = InvComponent->GetColumns() * TileSize;
	Y = InvComponent->GetRows() * TileSize;
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border->Slot);
	CanvasSlot->SetSize(FVector2D(X, Y));
	CreateLineSegments();
	Refresh();
	bInitzalized = true;
}

void UInventoryGridWidget::CreateLineSegments()
{
	float XPos;
	float YPos;
	FGridLines TempLine;
	FVector2D TopLeftOfGrid = Border->GetCachedGeometry().AbsoluteToLocal((FVector2D)(0, 0));
	for (int Cinc = 0; Cinc < InvComponent->GetColumns(); Cinc++)
	{
		XPos = Cinc * TileSize;
		TempLine.Start = (FVector2D(XPos, 0.0f) + TopLeftOfGrid);
		TempLine.End = (FVector2D(XPos, (InvComponent->GetRows() * TileSize)) + TopLeftOfGrid);
		Lines.push_back(TempLine);
	}
	for (int Rinc = 0; Rinc < InvComponent->GetRows(); Rinc++)
	{
		YPos = Rinc * TileSize;
		TempLine.Start = (FVector2D(0.0f, YPos) + TopLeftOfGrid);
		TempLine.End = (FVector2D((InvComponent->GetColumns() * TileSize), YPos) + TopLeftOfGrid);
		Lines.push_back(TempLine);
	}
}

void UInventoryGridWidget::Refresh()
{
	//recreate the items after the inventory has been changed
	Canvas->ClearChildren();
	AllItems = InvComponent->GetAllItems();
	ItemWidget.Empty();
	FGridTiles TempTile;
	MyPlayer = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	check(MyPlayer);
	UItemWidget* TempWidget;
	for (int inc = 0; inc < AllItems.Num(); inc++)
	{
		TempTile = AllItems[inc];
		TempWidget = CreateWidget<UItemWidget>(MyPlayer, ItemWidgetClass);
		TempWidget->SetVariables(InvComponent, TempTile.HeldItem, TileSize, TempTile.HeldItemIndex);
		Canvas->AddChild(TempWidget);
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TempWidget->Slot);
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetPosition(FVector2D((TempTile.X * TileSize), (TempTile.Y * TileSize)));
		ItemWidget.Add(TempWidget);
	}
}

void UInventoryGridWidget::SetWidgetRefrences(UBorder* BorderRef, UCanvasPanel* CanvasRef)
{
	Border = BorderRef;
	Canvas = CanvasRef;
}

int32 UInventoryGridWidget::NativePaint(const FPaintArgs& PaintArgs, const FGeometry& AllocatedGeometry, const FSlateRect& CullRect, FSlateWindowElementList& OutDrawElements, int32 LayerID, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(PaintArgs, AllocatedGeometry, CullRect, OutDrawElements, LayerID, InWidgetStyle, bParentEnabled);
	FPaintContext myContext(AllocatedGeometry, CullRect, OutDrawElements, LayerID, InWidgetStyle, bParentEnabled);
	if (!bHasScriptImplementedPaint)
	{
		for (int Vit = 0; Vit < Lines.size(); Vit++)
		{
			UWidgetBlueprintLibrary::DrawLine(myContext, (Lines[Vit].Start), (Lines[Vit].End), GridColour, true, 2.0f);
		}

		return FMath::Max(LayerID, myContext.MaxLayer);
	}
	return LayerID;
}

bool UInventoryGridWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UItemWidget* DropedItem = Cast<UItemWidget>(InOperation->Payload);
	if (InvComponent->ReAddedItem(DropedItem->ItemID))
	{
		//if item can be readded do nothing
	}
	else
	{
		InvComponent->SpawnItem(DropedItem->ItemID);
	}
	DropedItem->RemoveFromParent();
	return true;
}

bool UInventoryGridWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	FVector2D MousePos = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	MousePositionInTile(MousePos);
	UItemWidget* DropedItem = Cast<UItemWidget>(InOperation->Payload);
	FItemTable* TempRow = InvComponent->ReturnItemInfo(DropedItem->ItemID);
	int DimX = TempRow->SizeX;
	int DimY = TempRow->SizeY;
	//calculate which tile is closest
	if (bMouseRight == true)
	{
		DimX--;
		DimX = FMath::Clamp(DimX, 0, DimX);
	}
	if (bMouseDown == true)
	{
		DimY--;
		DimY = FMath::Clamp(DimY, 0, DimY);
	}
	FIntPoint TempPoint;
	TempPoint.X = DimX;
	TempPoint.Y = DimY;
	TempPoint = TempPoint / 2;
	MousePos = MousePos / TileSize;
	TempPoint = FIntPoint(MousePos.X, MousePos.Y) - TempPoint;
	InvComponent->SetDraggedItemTile(TempPoint);
	return true;
}


void UInventoryGridWidget::MousePositionInTile(FVector2D MousePos)
{
	//convert the mouse position to a tile index
	float TempX = MousePos.X;
	float ModX = fmod(TempX, TileSize);
	if (ModX > (TileSize / 2))
	{
		bMouseRight = true;
	}
	else
	{
		bMouseRight = false;
	}
	float TempY = MousePos.Y;
	float ModY = fmod(TempY, TileSize);
	if (ModY > (TileSize / 2))
	{
		bMouseDown = true;
	}
	else
	{
		bMouseDown = false;
	}
}

void UInventoryGridWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	if (bInitzalized == true)
	{
		if (InvComponent->GetIsDirty() == true)
		{
			//if item has been added, removed or changed location redraw the items
			Refresh();
			for (int Inc = 0; Inc < ItemWidget.Num(); Inc++)
			{
				ItemWidget[Inc]->Refresh();
			}
			InvComponent->SetIsDirty(false);
		}
	}
}