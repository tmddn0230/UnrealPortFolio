// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// TestVoice
#include "Misc/CoreMisc.h"
#include "Containers/Ticker.h"

#include "MyCharacter.generated.h"

// Voice
class UAudioComponent;
class USoundWaveProcedural;
class IVoiceCapture;
class IVoiceEncoder;
class IVoiceDecoder;
enum class EAudioEncodeHint : uint8;
// Manager
class UMyNetworkManager;
class UMyReplayManager;
// RumtimeAudioImporter
class UCapturableSoundWave;


// Define
#define VOICE_MAX_COMPRESSED_BUFFER 20 * 1024
#define VOICE_STARTING_REMAINDER_SIZE 1 * 1024

UCLASS()
class UNREALPORTFOLIO_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

private:


	UPROPERTY()	UAudioComponent* VoiceComp;
	UPROPERTY() USoundWaveProcedural* SoundStreaming;

	TSharedPtr<IVoiceCapture> VoiceCapture;
	TSharedPtr<IVoiceEncoder> VoiceEncoder;
	TSharedPtr<IVoiceDecoder> VoiceDecoder;

	FString DeviceName;

	EAudioEncodeHint         EncodeHint;              // Encoding Rules
	int32                    InputSampleRate;
	int32                    OutputSampleRate;
	int32                    NumInChannels;
	int32                    NumOutChannels;

	bool                     bLastWasPlaying;
	int32                    StarvedDataCount;        // Number of consecutive frames that playback has been starved. (why need this...?)

	TArray<uint8>            RawCaptureData;          // Before encoding
	int32                    MaxRawCaptureDataSize;   // channel * samplerate * sizeof(uint)
				             
	TArray<uint8>            CompressedData;          // After encoding
	int32                    MaxCompressedDataSize;   // Maximum size of a single encoded packet
				             
	TArray<uint8>            UncompressedData;        // After decoding
	int32                    MaxUncompressedDataSize; // Maximum size of a single decoded packet

	mutable FCriticalSection QueueLock;
	TQueue<TArray<uint8>>    CompressedDataQueue;     // After encoding data's queue (FROM UDP)
	TArray<uint8>            UncompressedDataQueue;   // After decoding data's queue
	int32                    CurrentUncompressedDataQueueSize;
	int32                    MaxUncompressedDataQueueSize;

	TArray<uint8>            Remainder;               // Remain data after encoding, remainder used next encoding
	int32                    MaxRemainderSize;
	int32                    LastRemainderSize;            
	uint64                   CachedSampleCount;

	bool bZeroInput;
	bool bUseDecompressed;                            // skip Encode/Decode for Test
	bool bZeroOutput;


/*
========================================================================================================
====        Record      ================================================================================
========================================================================================================
*/
	// udp
	UMyNetworkManager* netmgr;
	// rep
	UMyReplayManager* repmgr;

	FString VoiceFileName;
	int32 rep_TID;
	int32 rep_UID;
public:
	UPROPERTY()
    UCapturableSoundWave* CapturableSoundWave;

	
public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
/*
========================================================================================================
====     Voice Chat     ================================================================================
========================================================================================================
*/

	void InitVoiceCapture();                     // re init with current setting and realloc buffers
	void InitVoiceEncoder();					 // re init with current setting and realloc buffers
	void InitVoiceDecoder();					 // re init with current setting and realloc buffers
	void InitReplayManager();

	void Shutdown();

	void CleanupVoice();
	void CleanupAudioComponent();
	void CleanupQueue();

public:
	/**
	 * Callback from streaming audio when data is requested for playback
	 *
	 * @InProceduralWave : SoundWave requesting more data
	 * @SamplesRequired  : number of samples needed for immediate playback
	 */
	UFUNCTION()
	void GenerateData(USoundWaveProcedural* InProceduralWave, int32 SamplesRequired);

	UFUNCTION()
	void DecodedAndRun(const TArray<uint8>& buffer, int CompressedSize);

	FTimerHandle PlayVoiceTimerHandler;
	FTimerHandle DequeHandler;
	void VoiceCaptureTick();


/*
========================================================================================================
====        Record      ================================================================================
========================================================================================================
*/

// Bind and excute when beginplay
	UFUNCTION()
	void StartCapture();

	UFUNCTION()
	void RestartCapture();

	// Bind beginplay and excute when finish training
	UFUNCTION()
	void ExportCapture(int TID, int UID, FString TraineeCode);

	UFUNCTION()
	void FinishExportSound();

};
