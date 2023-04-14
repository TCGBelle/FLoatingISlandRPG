// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataObject(TEXT("/Script/Engine.DataTable'/Game/BelleAcademy/Datatypes/InventoryLookUpTable.InventoryLookUpTable'"));
	//if (ItemDataObject.Succeeded())
	//{
		//ItemDataTable = ItemDataObject.Object;
	//}
	// ...
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	int Temp;
	Temp = Columns * Rows;
	FItem TempItem;

	for (int Index = 0; Index < Temp; Index++)
	{
		TempItem.ID = -1;
		Items.push_back(TempItem);
		Reserved.push_back(TempItem);
	}
}

void UInventoryComponent::SetColumns(int Ref)
{
	Columns = Ref;
}

void UInventoryComponent::SetRows(int Ref)
{
	Rows = Ref;
}

int UInventoryComponent::GetColumns()
{
	return Columns;
}

int UInventoryComponent::GetRows()
{
	return Rows;
}

bool UInventoryComponent::TryAddItem(int PickUpID)
{
	FItem TempItem;
	TempItem.ID = PickUpID;
	if (CheckReserve(TempItem))
	{
		for (int Index = 0; Index < Items.size(); Index++)
		{
			if (IsRoomAvaliable(TempItem, Index))
			{
				AddItemAT(TempItem, Index);
				return true;
			}
		}
	}
	return false;
}

bool UInventoryComponent::IsRoomAvaliable(FItem Item, int TopLeftIndex)
{
	//sequence
	//loop over each tile that the item would occupy if item added at tile = to top left index
	//for eachindex
	FGridTiles TempTile = IndexToTile(TopLeftIndex);//convert index to a (x,y)
	FItemTable* TempRow = ReturnItemInfo(Item.ID);
	int XDim = TempRow->SizeX;
	int YDim = TempRow->SizeY;
	int TempIndex;
	if (Reserved[TopLeftIndex].ID == -1)
	{
		for (int IncX = TempTile.X; IncX <= (TempTile.X + (XDim - 1)); IncX++)
		{
			for (int IncY = TempTile.Y; IncY <= (TempTile.Y + (YDim - 1)); IncY++)
			{
				//is tile valid
				if (IncX >= 0 && IncY >= 0 && IncX < Columns && IncY < Rows)
				{
					//is the tileoccupied
					TempIndex = TileToIndex(IncX, IncY);
					if (Items[TempIndex].ID == -1)
					{
						//the tile is free and not outside the bounds of the grid. time to do nothing and check the next one
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
	}
	//if there is enough sapce, at a valid location, and the inventory at that location + all the space it would take up is empty (e.g -1) then can return true else return false
	return true;
}

FGridTiles UInventoryComponent::IndexToTile(int Index)
{
	int X;
	int Y;
	X = Index % Columns;
	Y = Index / Columns;
	//do maths for working out x and y
	FGridTiles TempTile;
	TempTile.X = X;
	TempTile.Y = Y;
	return TempTile;
}

FItemTable* UInventoryComponent::ReturnItemInfo(int ItemID)
{
	if (ItemDataTable)
	{
		FName RowName = FName(*FString::FromInt(ItemID));
		const TCHAR* ContextString(TEXT("Item Context"));
		ReturnedRow = ItemDataTable->FindRow<FItemTable>(RowName, ContextString, true);
	}
	return ReturnedRow;
}

int UInventoryComponent::GetItemAtIndex(int Index)
{
	return Items[Index].ID;
}

int UInventoryComponent::TileToIndex(int X, int Y)
{
	return (X + (Y * Columns));
}

void UInventoryComponent::AddItemAT(FItem ItemObj, int TopLeftIndex)
{
	//add item at the spoecified tile (into the ID array)
	FGridTiles TempTile = IndexToTile(TopLeftIndex);//convert index to a (x,y)
	FItemTable* TempRow = ReturnItemInfo(ItemObj.ID);
	int XDim = TempRow->SizeX;
	int YDim = TempRow->SizeY;
	int TempIndex;
	for (int IncX = TempTile.X; IncX <= (TempTile.X + (XDim - 1)); IncX++)
	{
		for (int IncY = TempTile.Y; IncY <= (TempTile.Y + (YDim - 1)); IncY++)
		{
			TempIndex = TileToIndex(IncX, IncY);
			Items[TempIndex].ID = ItemObj.ID;
		}
	}
	bIsDirty = true;
}

bool UInventoryComponent::CheckReserve(FItem ItemObj)
{
	// check if there is a reserved space for the item
	for (int Inc = 0; Inc < Reserved.size(); Inc++)
	{
		if (Reserved[Inc].ID == ItemObj.ID)
		{
			if (IsRoomAvaliable(ItemObj, Inc))
			{
				AddItemAT(ItemObj, Inc);
				return false;
			}
		}
	}
	return true;
}

bool UInventoryComponent::AddReserve(int ItemID)
{
	//reserve a slot in the inventory for a simmilar item picked up later
	int TempInt = TileToIndex(DraggedItemTopLeftTile.X, DraggedItemTopLeftTile.Y);
	FItem TempItem;
	TempItem.ID = ItemID;
	FGridTiles TempTile = IndexToTile(TempInt);//convert index to a (x,y)
	FItemTable* TempRow = ReturnItemInfo(TempItem.ID);
	int XDim = TempRow->SizeX;
	int YDim = TempRow->SizeY;
	int TempIndex;
	for (int IncX = TempTile.X; IncX <= (TempTile.X + (XDim - 1)); IncX++)
	{
		for (int IncY = TempTile.Y; IncY <= (TempTile.Y + (YDim - 1)); IncY++)
		{
			TempIndex = TileToIndex(IncX, IncY);
			Reserved[TempIndex].ID = TempItem.ID;
		}
	}
	return false;
}

TArray<FGridTiles> UInventoryComponent::GetAllItems()
{
	//return all items held by the inventory as a 1d array
	AllItems.Empty();
	FGridTiles TempTile;
	int TempIndex;
	std::vector<FItem> TempArray = Items;
	for (int index = 0; index < Items.size(); index++)
	{
		if (TempArray[index].ID != -1)
		{
			TempTile = IndexToTile(index);
			TempTile.HeldItem = TempArray[index].ID;
			TempTile.HeldItemIndex = index;
			AllItems.Add(TempTile);
			FItemTable* TempRow = ReturnItemInfo(TempArray[index].ID);
			int XDim = TempRow->SizeX;
			int YDim = TempRow->SizeY;
			for (int IncX = TempTile.X; IncX <= (TempTile.X + (XDim - 1)); IncX++)
			{
				for (int IncY = TempTile.Y; IncY <= (TempTile.Y + (YDim - 1)); IncY++)
				{
					TempIndex = TileToIndex(IncX, IncY);
					TempArray[TempIndex].ID = -1;
				}
			}
		}
	}
	return AllItems;
}

void UInventoryComponent::ItemRemoved(int ID, int Index)
{
	//remove item from the inventory (item array)
	FGridTiles TempTile;
	int TempIndex;
	TempTile = IndexToTile(Index);
	FItemTable* TempRow = ReturnItemInfo(ID);
	int XDim = TempRow->SizeX;
	int YDim = TempRow->SizeY;
	for (int IncX = TempTile.X; IncX <= (TempTile.X + (XDim - 1)); IncX++)
	{
		for (int IncY = TempTile.Y; IncY <= (TempTile.Y + (YDim - 1)); IncY++)
		{
			TempIndex = TileToIndex(IncX, IncY);
			Items[TempIndex].ID = -1;
		}
	}
	bIsDirty = true;
}

void UInventoryComponent::SpawnItem(int ID)
{
	//spawn the actor infront of the player
	FItemTable* TempRow = ReturnItemInfo(ID);
	ensure(TempRow->SpawnedClass);
	if (TempRow->SpawnedClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FVector SpawnLocation = GetOwner()->GetActorLocation();
			FVector ForwardVector = GetOwner()->GetActorForwardVector();
			FRotator Rotation = GetOwner()->GetActorRotation();
			ForwardVector = ForwardVector * 150;
			SpawnLocation += ForwardVector;
			FHitResult HitResult;
			FVector EndLocation = SpawnLocation - FVector(0, 0, -500);
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner());
			if (World->LineTraceSingleByChannel(HitResult, SpawnLocation, EndLocation, ECollisionChannel::ECC_Visibility, Params, FCollisionResponseParams()))
			{
				SpawnLocation = HitResult.Location;
			}
			FActorSpawnParameters ActorSpawnParams;
			World->SpawnActor<AActor>(TempRow->SpawnedClass, SpawnLocation, Rotation, ActorSpawnParams);
		}
	}
}

void UInventoryComponent::SetDraggedItemTile(FIntPoint NewTile)
{
	DraggedItemTopLeftTile = NewTile;
}

void UInventoryComponent::SetIsDirty(bool Dirt)
{
	bIsDirty = Dirt;
}

bool UInventoryComponent::GetIsDirty()
{
	return bIsDirty;
}

bool UInventoryComponent::ReAddedItem(int ID)
{
	//re add the item if its droped with in the confineds of the grid
	int TempInt = TileToIndex(DraggedItemTopLeftTile.X, DraggedItemTopLeftTile.Y);
	FItem TempItem;
	TempItem.ID = ID;
	if (IsRoomAvaliable(TempItem, TempInt))
	{
		AddItemAT(TempItem, TempInt);
		return true;
	}
	else
	{
		if (TryAddItem(TempItem.ID))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

