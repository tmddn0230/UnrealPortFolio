// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
// Network
#include "Network/MyNetworkManager.h"
// Protocol
#include "UPCore/Public/MyProtocol.h"

#include "MyReplayManager.generated.h"

USTRUCT(BlueprintType)
struct FMyReplayStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Replay) FString SelectedScenarioName;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Replay) FString SelectedScenarioFolderPath;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Replay) int32 SelectedTID;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Replay) int32 SelectedUID;

	TArray<int32> UserIDs;
	TArray<FString> TraineeRoles;
	TArray<FString> TraineeNames;

	FMyReplayStruct() {
		SelectedScenarioName = TEXT("");
		SelectedScenarioFolderPath = TEXT("");
		SelectedTID = 0;
	}

	void SetArray(const TArray<FString>& InArrayRole, const TArray<FString>& InArrayName, const TArray<int32>& InArrayUID) {
		TraineeNames = InArrayName;
		TraineeRoles = InArrayRole;
		UserIDs = InArrayUID;
	}

	TArray<FString> GetNames() { return TraineeNames; }
	TArray<FString> GetRoles() { return TraineeRoles; }
	TArray<int32>   GetUIDs() { return UserIDs; }
};

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UMyReplayManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	FMyReplayStruct ReplayInfo;
	
	
public:

	// Wav Import
	class USoundWave* LoadSoundWave(const FString& FilePath);
	UFUNCTION(BlueprintCallable)
	TArray<FString> ListAllWavFilesInFolder();
	
	USoundWave* MixSoundWaves(const TArray<USoundWave*>& SoundWaves);

};
