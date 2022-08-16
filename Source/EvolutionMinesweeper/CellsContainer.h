#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetworkHandler.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "Kismet/GameplayStatics.h" 
#include "CellsContainer.generated.h"

UCLASS()
class EVOLUTIONMINESWEEPER_API ACellsContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	ACellsContainer();

	UPROPERTY(BlueprintReadOnly)
	FMinesweeperRow Map;

	UPROPERTY(VisibleAnywhere)
	UPaperTileMapComponent* TileMapComponent;

	UPROPERTY(EditAnywhere)
	UPaperTileSet* TileSet;

	UFUNCTION(BlueprintCallable)
	void CreateTileMap(TArray<FMinesweeperRow> NewMap);

	UFUNCTION(BlueprintCallable)
	void UpdateMap(TArray<FMinesweeperRow> UpdatedMap);

	UFUNCTION(BlueprintCallable)
	FVector2D OnTileClicked();

protected:
	virtual void BeginPlay() override;
};