// Fill out your copyright notice in the Description page of Project Settings.


#include "Replay/MyReplayManager.h"

// Voice
#include "Sound/SoundWaveProcedural.h"
#include "Components/AudioComponent.h"


USoundWave* UMyReplayManager::LoadSoundWave(const FString& FilePath)
{
	USoundWave* SoundWave = NewObject<USoundWave>(USoundWave::StaticClass());

	TArray<uint8> RawFile;
	bool ck = FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);
	if (FFileHelper::LoadFileToArray(RawFile, *FilePath))
	{
		FWaveModInfo WaveInfo;
		if (WaveInfo.ReadWaveInfo(RawFile.GetData(), RawFile.Num()))
		{
			// Set WAV Info to SoundWave
			SoundWave->SetSampleRate(*WaveInfo.pSamplesPerSec);
			SoundWave->NumChannels = *WaveInfo.pChannels;
			SoundWave->Duration = static_cast<float>(*WaveInfo.pWaveDataSize) / *WaveInfo.pAvgBytesPerSec;
			SoundWave->RawPCMDataSize = *WaveInfo.pWaveDataSize;

			// Copy Audio Data
			SoundWave->RawPCMData = (uint8*)FMemory::Malloc(SoundWave->RawPCMDataSize);
			FMemory::Memcpy(SoundWave->RawPCMData, WaveInfo.SampleDataStart, SoundWave->RawPCMDataSize);

			// Set Additional Info
			SoundWave->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
			SoundWave->NumChannels = *WaveInfo.pChannels;

			// Set Cue Point
			for (const FWaveCue& WaveCue : WaveInfo.WaveCues)
			{
				FSoundWaveCuePoint NewCuePoint;
				NewCuePoint.CuePointID = WaveCue.CuePointID;
				NewCuePoint.Label = WaveCue.Label.IsEmpty() ? FString::Printf(TEXT("Cue_%d"), WaveCue.CuePointID) : WaveCue.Label;
				NewCuePoint.FramePosition = WaveCue.Position;
				NewCuePoint.FrameLength = WaveCue.SampleLength;
				SoundWave->CuePoints.Add(NewCuePoint);
			}
			return SoundWave;
		}
	}
	return nullptr;
}

TArray<FString> UMyReplayManager::ListAllWavFilesInFolder()
{
	TArray<FString> FoundFiles;
	TArray<FString> FileFullPathArray;
	// 폴더 내의 모든 .wav 파일 찾기
	FString FullPath = FPaths::ConvertRelativePathToFull(ReplayInfo.SelectedScenarioFolderPath);
	FString SearchPattern = FullPath / TEXT("*.wav");
	IFileManager::Get().FindFiles(FoundFiles, *SearchPattern, true, false);

	// 각 WAV 파일 로드
	for (const FString& FileName : FoundFiles)
	{
		FString FullFilePath = FullPath / FileName;
		FPaths::MakeStandardFilename(FullFilePath);

		FileFullPathArray.Add(FullFilePath);
	}

	return FileFullPathArray;
}

USoundWave* UMyReplayManager::MixSoundWaves(const TArray<USoundWave*>& SoundWaves)
{
	if (SoundWaves.Num() == 0) 	return nullptr;

	int32 MaxLength = 0; // Longest Length of AudioFiles
	int32 SampleRate = 48000;
	int32 Channel = 2;

	for (const USoundWave* wave : SoundWaves) {
		MaxLength = FMath::Max(MaxLength, wave->RawPCMDataSize);
	}

	TArray<float> MixedBuffer;
	MixedBuffer.SetNumZeroed(MaxLength / sizeof(int16));

	for (const USoundWave* Wave : SoundWaves) {
		const int16* RawData = reinterpret_cast<const int16*>(Wave->RawPCMData);
		int32 NumSamples = Wave->RawPCMDataSize / sizeof(int16);

		for (int32 i = 0; i < NumSamples; ++i) {
			MixedBuffer[i] += static_cast<float>(RawData[i]) / 32768.0f;
		}
	}

	float MaxAmplitude = 0.0f;
	for (float& Sample : MixedBuffer) {
		MaxAmplitude = FMath::Max(MaxAmplitude, FMath::Abs(Sample));
	}

	if (MaxAmplitude > 1.0f) {
		for (float& Sample : MixedBuffer) {
			Sample /= MaxAmplitude;
		}
	}
	USoundWave* MixedSoundWave = NewObject<USoundWave>(USoundWave::StaticClass());
	MixedSoundWave->SetSampleRate(SampleRate);
	MixedSoundWave->NumChannels = Channel;
	MixedSoundWave->Duration = static_cast<float>(MaxLength) / (SampleRate * Channel * sizeof(int16));
	MixedSoundWave->RawPCMDataSize = MaxLength;

	return MixedSoundWave;
}

//// 믹싱된 데이터를 int16 형식으로 변환
	//MixedSoundWave->RawPCMData = (uint8*)FMemory::Malloc(MaxLength);
	//int16* DestData = reinterpret_cast<int16*>(MixedSoundWave->RawPCMData);
	//for (int32 i = 0; i < MixedBuffer.Num(); ++i)
	//{
	//	DestData[i] = static_cast<int16>(MixedBuffer[i] * 32767.0f);
	//}
	//
	//// 믹싱된 데이터를 uint8 로 변환
	//TArray<uint8> ImportedBuffer;
	//ImportedBuffer.SetNum(MixedSoundWave->RawPCMDataSize);
	//FMemory::Memcpy(ImportedBuffer.GetData(), MixedSoundWave->RawPCMData, MixedSoundWave->RawPCMDataSize);

// Import Buffer
	//URuntimeAudioImporterLibrary* Importer = URuntimeAudioImporterLibrary::CreateRuntimeAudioImporter();
	//Importer->OnResult.AddDynamic(this, &URtReplayManager::Finish_Import);
	//Importer->ImportAudioFromRAWBuffer(ImportedBuffer, ERuntimeRAWAudioFormat::Int16, SampleRate, Channel);
