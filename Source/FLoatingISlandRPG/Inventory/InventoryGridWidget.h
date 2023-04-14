// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include <vector>
#include "InventoryGridWidget.generated.h"

/**
 * 
 */
UCLASS()
class FLOATINGISLANDRPG_API UInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitalizeGrid(UInventoryComponent* InvComp, float Tilefloat);
	void Refresh();
	UFUNCTION(BlueprintCallable)
		void SetWidgetRefrences(UBorder* BorderRef, UCanvasPanel* CanvasRef);
private:
	virtual int32 NativePaint(const FPaintArgs& PaintArgs,
		const FGeometry& AllocatedGeometry,
		const FSlateRect& CullRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerID,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void MousePositionInTile(FVector2D MousePos);
	void CreateLineSegments();
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	bool bMouseDown;
	bool bMouseRight;

	UPROPERTY(EditAnywhere, Category = "Grid")
		FLinearColor GridColour;
	UPROPERTY(EditAnywhere, Category = "widgets")
		UCanvasPanel* Canvas;
	UPROPERTY(EditAnywhere, Category = "widgets")
		UBorder* Border;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UItemWidget> ItemWidgetClass;
	UPROPERTY()
		TArray<class UItemWidget*> ItemWidget;
	UPROPERTY()
		APlayerController* MyPlayer;
private:
	UPROPERTY()
		UInventoryComponent* InvComponent;
	float TileSize;
	std::vector<FGridLines> Lines;
	TArray<FGridTiles> AllItems;
	bool bInitzalized;
};


USTRUCT(BlueprintType)
struct FGridLines
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		FVector2D Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		FVector2D End;
	UPROPERTY()
		UObject* SafeObjectPointer;
};
