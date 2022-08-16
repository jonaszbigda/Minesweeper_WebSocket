#include "NetworkHandler.h"

ANetworkHandler::ANetworkHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANetworkHandler::BeginPlay()
{
	Super::BeginPlay();
	SetupWebSocket();
}

void ANetworkHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Initiate new game

void ANetworkHandler::NewGame(int32 Level)
{
	if (!Socket->IsConnected())
	{
		return;
	}

	const FString LevelAsString = FString::FromInt(Level);
	FString Message = "new ";
	Message.Append(LevelAsString);
	WaitForNewMap = true;

	Socket->Send(Message);
}

// Open field

void ANetworkHandler::OpenField(int32 X, int32 Y)
{
	if (!Socket->IsConnected())
	{
		return;
	}

	FString Message = TEXT("open ");
	Message.Append(FString::FromInt(X));
	Message.Append(TEXT(" "));
	Message.Append(FString::FromInt(Y));

	Socket->Send(Message);
}

// Parse map from string to project's map format

TArray<FMinesweeperRow> ANetworkHandler::ParseMap(FString InputString)
{
	FString NoPrefix = InputString;
	FString Prefix = "";
	TArray<FMinesweeperRow> Result;
	NoPrefix.Split(TEXT("\n"), &Prefix, &NoPrefix);

	// Rows

	while (NoPrefix.Contains(TEXT("\n")))
	{
		TArray<TEnumAsByte<EFieldType>> TempFields;
		FString TempRowString;
		NoPrefix.Split(TEXT("\n"), &TempRowString, &NoPrefix);

		// Columns

		for (int32 i = 0; i < TempRowString.Len(); ++i)
		{
			FString Character = TempRowString.Mid(i, 1);
			if (Character == "0") {
				TempFields.Add(EFieldType::Empty);
			}
			else if (Character == "1") {
				TempFields.Add(EFieldType::OnePoint);
			}
			else if (Character == "2") {
				TempFields.Add(EFieldType::TwoPoints);
			}
			else if (Character == "3") {
				TempFields.Add(EFieldType::ThreePoints);
			}
			else if (Character == "4") {
				TempFields.Add(EFieldType::FourPoints);
			}
			else
			{
				TempFields.Add(EFieldType::Undiscovered);
			}
		}

		FMinesweeperRow TempResult;
		TempResult.Fields = TempFields;
		Result.Add(TempResult);
	}

	return Result;
}

// Setup WebSocket

void ANetworkHandler::SetupWebSocket() 
{
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	// Create socket
	Socket = FWebSocketsModule::Get().CreateWebSocket(url, protocol);

	// Binds
	Socket->OnConnected().AddLambda([this]() -> void {
		IsConnected = true;
	});

	Socket->OnMessage().AddLambda([this](const FString &Message) -> void {
		ANetworkHandler::HandleMessageRecieved(Message);
	});

	// Connect

	Socket->Connect();
}

// Message handler

void ANetworkHandler::HandleMessageRecieved(FString Message)
{
	FString MessageType;
	FString MessageContent;
	Message.Split(TEXT(":"), &MessageType, &MessageContent);

	if (MessageType == TEXT("new"))
	{
		Socket->Send(TEXT("map"));
	}
	else if (MessageType == TEXT("open"))
	{
		if (MessageContent == TEXT(" You lose"))
		{
			YouLose();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Field opened"));
			Socket->Send(TEXT("map"));
		}
	}
	else if (MessageType == TEXT("map"))
	{
		TArray<FMinesweeperRow> TempMap = ANetworkHandler::ParseMap(MessageContent);
		Map = TempMap;
		UpdateMap();
	} 
	else {
		UE_LOG(LogTemp, Warning, TEXT("Unrecognized response"));
	}
}