// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/MyGameModeBase.h"
#include "MyGameMode.generated.h"

class AMyPlayerController;
class AMyPlayerState;
class AMyGameState;

UCLASS()
class UNREALPORTFOLIO_API AMyGameMode : public AMyGameModeBase
{
	GENERATED_BODY()
// Delegate

private:

	ENetMode Net_Mode;

	static int32 LoginCount;
	TArray<AMyPlayerController*> Trainee_Array;
	TArray<AMyPlayerController*> Observer_Array;
	AMyPlayerController*         Control_Controller;




public:
	AMyGameMode(const FObjectInitializer& ObjectInitializer);

	// Get
	AMyGameState*        Get_GameState();
	AMyPlayerController* Get_PlayerController();
	AMyPlayerController* Get_PlayerController(int32 InPlayerId); 
	AMyPlayerController* Get_TraineeController(int32 InPlayerId); 

	// Overriding
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void InitGameState() override;
	virtual void PreInitializeComponents() override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void StartPlay() override;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT(""));

};
