// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyGameManager.generated.h"


// Framework
class AMyPlayerController;
class AMyPlayerState;
class AMyGameState;


UCLASS()
class UNREALPORTFOLIO_API UMyGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UMyGameManager();

	// Main Flow
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void Init_Game(const FString& MapName, const FString& Options, FString& ErrorMessage, bool bGamemode);

};
