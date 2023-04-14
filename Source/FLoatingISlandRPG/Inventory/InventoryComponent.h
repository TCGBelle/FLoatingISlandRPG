// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include <List>
#include <vector>
#include "Materials/MaterialInterface.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLOATINGISLANDRPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		bool TryAddItem(int PickUpID);
	int GetColumns();
	int GetRows();
	TArray <FGridTiles> GetAllItems();
	bool ReAddedItem(int ID);
	FItemTable* ReturnItemInfo(int ItemID);
	void ItemRemoved(int ID, int Index);
	bool AddReserve(int ItemID);
	void SpawnItem(int ID);
	void SetDraggedItemTile(FIntPoint NewTIle);
	void SetIsDirty(bool Dirt);
	bool GetIsDirty();
private:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetColumns(int Ref);
	void SetRows(int Ref);
	bool IsRoomAvaliable(FItem Item, int TopLeftIndex);
	FGridTiles IndexToTile(int Index);
	int GetItemAtIndex(int Index);
	int TileToIndex(int X, int Y);
	void AddItemAT(FItem ItemObj, int TopLeftIndex);
	bool CheckReserve(FItem ItemObj);
private:
	UPROPERTY(EditAnywhere, Category = "Constants")
		int Columns;
	UPROPERTY(EditAnywhere, Category = "Constants")
		int Rows;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		class UDataTable* ItemDataTable;
	FItemTable* CurrRow;
	std::list<int> HeldItemIds;
	std::vector<FGridTiles> Tiles;
	std::vector<FItem> Items;
	std::vector<FItem> Reserved;
	FItemTable* ReturnedRow;
	TArray <FGridTiles> AllItems;
	FIntPoint DraggedItemTopLeftTile; //the top left most tile the item takes up
	bool bIsDirty;
};

USTRUCT(BlueprintType)
struct FItemTable :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int SizeX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int SizeY;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UMaterialInterface* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class AActor> SpawnedClass;
};

USTRUCT(BlueprintType)
struct FGridTiles
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		int X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		int Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		int HeldItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		int HeldItemIndex;
};

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item")
		int ID;
};