#include "CellsContainer.h"

ACellsContainer::ACellsContainer()
{
	PrimaryActorTick.bCanEverTick = true;
	TileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("TileMapComponent"));
	RootComponent = TileMapComponent;
}

void ACellsContainer::BeginPlay()
{
	Super::BeginPlay();
}

// Create TileMap from supplied map

void ACellsContainer::CreateTileMap(TArray<FMinesweeperRow> NewMap)
{
		int32 Size = NewMap.Num();
		TileMapComponent->CreateNewTileMap(Size, Size, 32);

		// Go through every cell and set it to "Undiscovered"

		for (int32 row = 0; row < Size; ++row)
		{
			for (int32 col = 0; col < Size; ++col)
			{
				FPaperTileInfo TileInfo;
				TileInfo.TileSet = TileSet;
				TileInfo.PackedTileIndex = 5;
				
				TileMapComponent->SetTile(col, row, 0, TileInfo);
			}
		}

		// Center the actor

		FVector NewLocation;
		NewLocation.X = ((Size / 2.0f) * 32.0f) - 16;
		NewLocation.Y = ((Size / 2.0f) * -32.0f) + 16;
		NewLocation.Z = 0.f;
		SetActorLocation(NewLocation);
		TileMapComponent->RebuildCollision();

	return;
}

void ACellsContainer::UpdateMap(TArray<FMinesweeperRow> UpdatedMap)
{
	int32 Size = UpdatedMap.Num();

	// Rows

	for (int32 row = 0; row < Size; ++row)
	{
		FMinesweeperRow ThisRow = UpdatedMap[row];

		// Columns

		for (int32 col = 0; col < ThisRow.Fields.Num(); ++col)
		{
			EFieldType FieldType = ThisRow.Fields[col].GetValue();
			FPaperTileInfo TileInfo;
			TileInfo.TileSet = TileSet;

			// Set corresponding tileIndex

			if (FieldType == Undiscovered) {
				TileInfo.PackedTileIndex = 5;
			}
			else if (FieldType == Empty)
			{
				TileInfo.PackedTileIndex = 4;
			}
			else if (FieldType == OnePoint)
			{
				TileInfo.PackedTileIndex = 0;
			}
			else if (FieldType == TwoPoints)
			{
				TileInfo.PackedTileIndex = 1;
			}
			else if (FieldType == ThreePoints)
			{
				TileInfo.PackedTileIndex = 2;
			}
			else if (FieldType == FourPoints)
			{
				TileInfo.PackedTileIndex = 3;
			}

			TileMapComponent->SetTile(col, row, 0, TileInfo);
		}
	}

	// Rebuild collision for TileMap

	TileMapComponent->RebuildCollision();

	return;
}

// On Tile Clicked (calculate clicked tile)

FVector2D ACellsContainer::OnTileClicked()
{
	FHitResult HitResult;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	FVector2D Result = (0.0f, 0.0f);
	FVector2D HitPoint;
	HitPoint.X = HitResult.Location.X - GetActorLocation().X - 16.0f;
	HitPoint.Y = HitResult.Location.Y - GetActorLocation().Y + 16.0f;

	Result.Y = (HitPoint.X / 32.0f) * -1.0f;
	Result.X = HitPoint.Y / 32.0f; 

	return Result;
}