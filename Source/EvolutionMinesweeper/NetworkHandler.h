#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "Modules/ModuleManager.h"
#include "NetworkHandler.generated.h"

UENUM(BlueprintType)
enum EFieldType {
	Undiscovered UMETA(DisplayName = "Undiscovered"),
	Empty UMETA(DisplayName = "Empty"),
	OnePoint UMETA(DisplayName = "1 point"),
	TwoPoints UMETA(DisplayName = "2 Points"),
	ThreePoints UMETA(DisplayName = "3 Points"),
	FourPoints UMETA(DisplayName = "4 Points"),
	Mine UMETA(DisplayName = "Mine"),
};

USTRUCT(BlueprintType)
struct FMinesweeperRow
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EFieldType>> Fields;
};

UCLASS()
class EVOLUTIONMINESWEEPER_API ANetworkHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetworkHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TSharedPtr<IWebSocket> Socket;

	UFUNCTION()
	void SetupWebSocket();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Data types

	UPROPERTY(BlueprintReadOnly)
	bool IsConnected = false;

	UPROPERTY()
	FString url = TEXT("wss://hometask.eg1236.com/game1/");

	UPROPERTY()
	FString protocol = TEXT("wss");

	UPROPERTY(BlueprintReadOnly)
	TArray<FMinesweeperRow> Map;

	UPROPERTY(BlueprintReadWrite)
	bool WaitForNewMap = false;

	// Methods

	UFUNCTION(BlueprintCallable)
	void OpenField(int32 X, int32 Y);

	UFUNCTION(BlueprintCallable)
	void NewGame(int32 Level);

	UFUNCTION()
	TArray<FMinesweeperRow> ParseMap(FString InputString);

	UFUNCTION()
	void HandleMessageRecieved(FString Message);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMap();

	UFUNCTION(BlueprintImplementableEvent)
	void YouLose();
};
