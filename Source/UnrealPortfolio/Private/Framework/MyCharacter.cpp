// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MyCharacter.h"
#include "Components/AudioComponent.h"

#include "Network/MyNetworkManager.h"
#include "OnlineSubsystemUtils.h"
#include "VoiceModule.h"
#include "Voice.h"
#include "Sound/SoundWaveProcedural.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

//#include "Sound/CapturableSoundWave.h"
//#include "RuntimeAudioImporterLibrary.h"
//#include "RuntimeAudioExporter.h"

#if !UE_BUILD_SHIPPING
#define VOICE_BUFFER_CHECK(Buffer, Size) \
	check(Buffer.Num() >= (int32)(Size))
#else
#define VOICE_BUFFER_CHECK(Buffer, Size)
#endif

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 변수 초기화
	VoiceComp = nullptr;
	SoundStreaming = nullptr;
	VoiceCapture = nullptr;
	VoiceEncoder = nullptr;
	VoiceDecoder = nullptr;
	EncodeHint = UVOIPStatics::GetAudioEncodingHint();
	InputSampleRate = UVOIPStatics::GetVoiceSampleRate();
	OutputSampleRate = UVOIPStatics::GetVoiceSampleRate();
	NumInChannels = UVOIPStatics::GetVoiceNumChannels();
	NumOutChannels = UVOIPStatics::GetVoiceNumChannels();
	bLastWasPlaying = false;
	StarvedDataCount = 0;
	MaxRawCaptureDataSize = 0;
	MaxCompressedDataSize = 0;
	MaxUncompressedDataSize = 0;
	CurrentUncompressedDataQueueSize = 0;
	MaxUncompressedDataQueueSize = 0;
	MaxRemainderSize = 0;
	LastRemainderSize = 0;
	CachedSampleCount = 0;
	bZeroInput = false;        // 해당 부분은 제어해볼 필요 있음
	bUseDecompressed = true;
	bZeroOutput = false;


	// Init 
	DeviceName = TEXT("");
	EncodeHint = EAudioEncodeHint::VoiceEncode_Audio;
	InputSampleRate = 48000;
	OutputSampleRate = 48000;
	NumInChannels = 1;
	NumOutChannels = 1;

	InitVoiceCapture();
	InitVoiceEncoder();
	InitVoiceDecoder();
	InitReplayManager();

	GetWorld()->GetTimerManager().SetTimer(PlayVoiceTimerHandler, this, &AMyCharacter::VoiceCaptureTick, 0.02f, true, 0.1f);

	if (auto* Gi = GetGameInstance()) {
		if (Gi) {
			netmgr = Gi->GetSubsystem<UMyNetworkManager>();
		}
	}
}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Shutdown();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::InitVoiceCapture()
{
	VoiceCapture = FVoiceModule::Get().CreateVoiceCapture(DeviceName, InputSampleRate, NumInChannels);
	if (VoiceCapture.IsValid())
	{
		MaxRawCaptureDataSize = VoiceCapture->GetBufferSize();

		RawCaptureData.Empty(MaxRawCaptureDataSize);
		RawCaptureData.AddUninitialized(MaxRawCaptureDataSize);

		VoiceCapture->Start();
	}
}

void AMyCharacter::InitVoiceEncoder()
{
	if (VoiceCapture.IsValid())
	{
		VoiceEncoder = FVoiceModule::Get().CreateVoiceEncoder(InputSampleRate, NumInChannels, EncodeHint);
		if (VoiceEncoder.IsValid())
		{
			MaxRemainderSize = VOICE_STARTING_REMAINDER_SIZE;
			LastRemainderSize = 0;
			MaxCompressedDataSize = VOICE_MAX_COMPRESSED_BUFFER;

			CompressedData.Empty(MaxCompressedDataSize);
			CompressedData.AddUninitialized(MaxCompressedDataSize);

			Remainder.Empty(MaxRemainderSize);
			Remainder.AddUninitialized(MaxRemainderSize);
		}
	}
}

void AMyCharacter::InitVoiceDecoder()
{
	if (VoiceCapture.IsValid())
	{
		VoiceDecoder = FVoiceModule::Get().CreateVoiceDecoder(OutputSampleRate, NumOutChannels);
		if (VoiceDecoder.IsValid())
		{
			MaxUncompressedDataSize = NumOutChannels * OutputSampleRate * sizeof(uint16);

			UncompressedData.Empty(MaxUncompressedDataSize);
			UncompressedData.AddUninitialized(MaxUncompressedDataSize);

			MaxUncompressedDataQueueSize = MaxUncompressedDataSize * 5;
			{
				FScopeLock ScopeLock(&QueueLock);
				UncompressedDataQueue.Empty(MaxUncompressedDataQueueSize);
			}
		}
	}
}

void AMyCharacter::InitReplayManager()
{
	//auto* gi = GetGameInstance();
	//if (gi) {
	//	repmgr = gi->GetSubsystem<UMyReplayManager>();
	//	if (repmgr) {
	//		repmgr->InitManager();
	//	}
	//}
}

void AMyCharacter::Shutdown()
{
	RawCaptureData.Empty();
	CompressedData.Empty();
	UncompressedData.Empty();
	Remainder.Empty();

	{
		FScopeLock ScopeLock(&QueueLock);
		UncompressedDataQueue.Empty();
	}

	CleanupVoice();
	CleanupAudioComponent();
}

void AMyCharacter::CleanupVoice()
{
	if (VoiceCapture.IsValid())
	{
		VoiceCapture->Shutdown();
		VoiceCapture = nullptr;
	}

	VoiceEncoder = nullptr;
	VoiceDecoder = nullptr;
}

void AMyCharacter::CleanupAudioComponent()
{
	if (VoiceComp != nullptr)
	{
		VoiceComp->Stop();

		SoundStreaming->OnSoundWaveProceduralUnderflow.Unbind();
		SoundStreaming = nullptr;

		VoiceComp->RemoveFromRoot();
		VoiceComp = nullptr;

		bLastWasPlaying = false;
	}
}

void AMyCharacter::CleanupQueue()
{
	FScopeLock ScopeLock(&QueueLock);
	UncompressedDataQueue.Reset();
}

void AMyCharacter::GenerateData(USoundWaveProcedural* InProceduralWave, int32 SamplesRequired)
{
	const int32 SampleSize = sizeof(uint16) * NumOutChannels;
	{
		FScopeLock ScopeLock(&QueueLock);
		CurrentUncompressedDataQueueSize = UncompressedDataQueue.Num();
		const int32 AvailableSamples = CurrentUncompressedDataQueueSize / SampleSize;
		if (AvailableSamples >= SamplesRequired)
		{
			InProceduralWave->QueueAudio(UncompressedDataQueue.GetData(), AvailableSamples * SampleSize);
			UncompressedDataQueue.RemoveAt(0, AvailableSamples * SampleSize, false);
			CurrentUncompressedDataQueueSize -= (AvailableSamples * SampleSize);

			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT(" GenerateData : QueueAudio "));
		}
	}
}

void AMyCharacter::DecodedAndRun(const TArray<uint8>& buffer, int CompressedSize)
{
	UE_LOG(LogTemp, Log, TEXT("CurrentUncompressedDataQueueSize %d!"), CurrentUncompressedDataQueueSize);

	uint32 UncompressedDataSize = 0;
	if (VoiceDecoder.IsValid() && CompressedSize > 0)
	{
		UncompressedDataSize = MaxUncompressedDataSize;
		VoiceDecoder->Decode(buffer.GetData(), CompressedSize, UncompressedData.GetData(), UncompressedDataSize);
		VOICE_BUFFER_CHECK(UncompressedData, UncompressedDataSize);
	}

	const uint8* VoiceDataPtr = nullptr;
	uint32 VoiceDataSize = 0;

	if (bUseDecompressed)
	{
		if (UncompressedDataSize > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Queuing uncompressed data! %d"), UncompressedDataSize);
			if (bZeroOutput)
			{
				FMemory::Memzero((uint8*)UncompressedData.GetData(), UncompressedDataSize);
			}

			VoiceDataSize = UncompressedDataSize;
			VoiceDataPtr = UncompressedData.GetData();
		}
	}

	if (VoiceDataPtr && VoiceDataSize > 0)
	{
		FScopeLock ScopeLock(&QueueLock);

		const int32 OldSize = UncompressedDataQueue.Num();
		const int32 AmountToBuffer = (OldSize + (int32)VoiceDataSize);
		if (AmountToBuffer <= MaxUncompressedDataQueueSize)
		{
			UncompressedDataQueue.AddUninitialized(VoiceDataSize);

			VOICE_BUFFER_CHECK(UncompressedDataQueue, AmountToBuffer);
			FMemory::Memcpy(UncompressedDataQueue.GetData() + OldSize, VoiceDataPtr, VoiceDataSize);
			CurrentUncompressedDataQueueSize += VoiceDataSize;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UncompressedDataQueue Overflow!"));
		}
	}

	if (CurrentUncompressedDataQueueSize > (MaxUncompressedDataSize / 2))
	{
		VoiceComp->Play();
	}
}

void AMyCharacter::VoiceCaptureTick()
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_FTVoice_Tick);



	if (VoiceCapture.IsValid())
	{
		if (VoiceComp == nullptr)
		{
			VoiceComp = NewObject<UAudioComponent>(this);
			VoiceComp->SetupAttachment(RootComponent);
			VoiceComp->bAutoActivate = true;
			VoiceComp->bAlwaysPlay = true;

			if (SoundStreaming == nullptr)
			{
				SoundStreaming = NewObject<USoundWaveProcedural>();
				SoundStreaming->SetSampleRate(48000);
				SoundStreaming->NumChannels = 1;
				SoundStreaming->Duration = INDEFINITELY_LOOPING_DURATION;
				SoundStreaming->SoundGroup = SOUNDGROUP_Voice;
				SoundStreaming->bLooping = false;
				SoundStreaming->bProcedural = true;
				VoiceComp->Sound = SoundStreaming;

				// Bind the GenerateData callback with FOnSoundWaveProceduralUnderflow object
				SoundStreaming->OnSoundWaveProceduralUnderflow.BindUFunction(this, TEXT("GenerateData"));
			}
		}

		if (VoiceComp)
		{
			check(SoundStreaming);

			bool bIsPlaying = VoiceComp->IsPlaying();
			if (bIsPlaying != bLastWasPlaying)
			{
				UE_LOG(LogTemp, Log, TEXT("VOIP audio component %s playing!"), bIsPlaying ? TEXT("is") : TEXT("is not"));
				bLastWasPlaying = bIsPlaying;
			}
			// 이전에 인코딩하고 남은 데이터 처리
			StarvedDataCount = (!bIsPlaying || (SoundStreaming->GetAvailableAudioByteCount() != 0)) ? 0 : (StarvedDataCount + 1);
			if (StarvedDataCount > 1)
			{
				UE_LOG(LogTemp, Log, TEXT("VOIP audio component starved %d frames!"), StarvedDataCount);
			}

			bool bDoWork = false;
			uint32 TotalVoiceBytes = 0;

			uint32 NewVoiceDataBytes = 0;
			EVoiceCaptureState::Type MicState = VoiceCapture->GetCaptureState(NewVoiceDataBytes);
			if (MicState == EVoiceCaptureState::Ok && NewVoiceDataBytes > 0)
			{
				UE_LOG(LogTemp, Log, TEXT("Getting data! %d"), NewVoiceDataBytes);
				if (LastRemainderSize > 0)
				{
					// Add back any data from the previous frame
					VOICE_BUFFER_CHECK(RawCaptureData, LastRemainderSize);
					FMemory::Memcpy(RawCaptureData.GetData(), Remainder.GetData(), LastRemainderSize);
				}

				// Add new data at the beginning of the last frame
				uint64 SampleCount;
				MicState = VoiceCapture->GetVoiceData(RawCaptureData.GetData() + LastRemainderSize, NewVoiceDataBytes, NewVoiceDataBytes, SampleCount);
				TotalVoiceBytes = NewVoiceDataBytes + LastRemainderSize;

				// Check to make sure this buffer has a valid, chronological buffer count.
				if (SampleCount <= CachedSampleCount)
				{
					UE_LOG(LogTemp, Log, TEXT(
						"Out of order or ambiguous sample count detected! This sample count: %lu Previous sample count: %lu"),
						SampleCount, CachedSampleCount);
				}

				CachedSampleCount = SampleCount;

				VOICE_BUFFER_CHECK(RawCaptureData, TotalVoiceBytes);
				bDoWork = (MicState == EVoiceCaptureState::Ok);
			}

			if (bDoWork && TotalVoiceBytes > 0)
			{
				// ZERO INPUT
				if (bZeroInput)
				{
					FMemory::Memzero(RawCaptureData.GetData(), TotalVoiceBytes);
				}
				// ZERO INPUT END

				// COMPRESSION BEGIN
				uint32 CompressedDataSize = 0;
				if (VoiceEncoder.IsValid())
				{
					CompressedDataSize = MaxCompressedDataSize;
					LastRemainderSize = VoiceEncoder->Encode(RawCaptureData.GetData(), TotalVoiceBytes, CompressedData.GetData(), CompressedDataSize);
					VOICE_BUFFER_CHECK(RawCaptureData, CompressedDataSize);

					if (LastRemainderSize > 0)
					{
						if (LastRemainderSize > MaxRemainderSize)
						{
							UE_LOG(LogTemp, Verbose, TEXT("Overflow!"));
							Remainder.AddUninitialized(LastRemainderSize - MaxRemainderSize);
							MaxRemainderSize = Remainder.Num();
						}

						VOICE_BUFFER_CHECK(Remainder, LastRemainderSize);
						FMemory::Memcpy(Remainder.GetData(), RawCaptureData.GetData() + (TotalVoiceBytes - LastRemainderSize), LastRemainderSize);
					}

					if (CompressedDataSize > 0)
					{
						UE_LOG(LogTemp, Log, TEXT("VOIP audio component Encoded %d !"), CompressedDataSize);
					}

				}
				// COMPRESSION END

				// UDP
				netmgr->SendPacket_UDP(CompressedData, CompressedDataSize);
			}
		}
	}
}


void AMyCharacter::StartCapture()
{
	//if (CapturableSoundWave == nullptr) {
	//	CapturableSoundWave = NewObject<UCapturableSoundWave>();
	//	CapturableSoundWave->CreateCapturableSoundWave();
	//}
	//
	//if (CapturableSoundWave) {
	//	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT(" CapturableSoundWave Create "));
	//}
	//else
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT(" CapturableSoundWave Create Failed"));
	//	return;
	//}
	//
	//// Device Number : 0
	//if (CapturableSoundWave->StartCapture(0))
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT(" CapturableSoundWave Start "));
	//}
}

void AMyCharacter::RestartCapture()
{
	//if (CapturableSoundWave)
	//{
	//	CapturableSoundWave->StopCapture();
	//}
}

void AMyCharacter::ExportCapture(int TID, int UID, FString TraineeCode)
{
//	// Set Params for upload file
//	rep_TID = TID;
//	rep_UID = UID;
//	VoiceFileName = TraineeCode;
//
//	if (CapturableSoundWave)
//	{
//		CapturableSoundWave->StopCapture();
//
//		URuntimeAudioExporter* RuntimeAudioExporter = NewObject<URuntimeAudioExporter>();
//		if (RuntimeAudioExporter)
//		{
//			FOnAudioExportToFileResultNative result;
//			result.BindUFunction(this, TEXT("FinishExportSound"));
//			FRuntimeAudioExportOverrideOptions overrideoption;
//			FString baseDir;
//#if WITH_EDITOR
//			baseDir = FPaths::ProjectDir();
//#else
//			baseDir = FPaths::LaunchDir();
//#endif
//			FString Directory_path = FString::Printf(TEXT("TrainingData/RecordingData/%d/%s.WAV"), TID, *TraineeCode);
//			FString FilePath = baseDir + Directory_path;
//			FPaths::MakeStandardFilename(FilePath);
//
//			RuntimeAudioExporter->ExportSoundWaveToFile(CapturableSoundWave, FilePath, ERuntimeAudioFormat::Wav, 100, overrideoption, result);
//		}
//	}
}

void AMyCharacter::FinishExportSound()
{
	//if (repmgr) {
	//	FString str_TID = FString::FromInt(rep_TID);
	//	FString str_UID = FString::FromInt(rep_UID);
	//	repmgr->Upload_VoiceFile(str_TID, str_UID, VoiceFileName);
	//}
}
