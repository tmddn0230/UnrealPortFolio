// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Replay/My_ReplayViewer.h"

#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
// ReplayManager
#include "Replay/MyReplayManager.h"
// RAI
#include "RuntimeAudioImporterLibrary.h"
#include "RuntimeAudioExporter.h"
#include "RuntimeAudioImporter.h"

void UMy_ReplayViewer::OnSliderValueChanged(float Value)
{
	if (ImportedSoundWave) {
		float PlayTime = Value * ImportedSoundWave->GetDuration();
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::Printf(TEXT("Playing Time : %f"), PlayTime));
		ImportedSoundWave->RewindPlaybackTime(PlayTime);
	}
}

void UMy_ReplayViewer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	if (ImportedSoundWave)
	{
	   if (ImportedSoundWave->IsPlaying(GetWorld()) && ReplaySlider_var) {
	   
	   	  if (bIsPause) {
	   	  // 재생중이 아님
	   	  }
	   	  else {
	   	  	CurrentPlaybackTime = ImportedSoundWave->GetPlaybackTime();
	   	  	TotalDuration = ImportedSoundWave->GetDuration();
	   	  
	   	  	SliderValue = CurrentPlaybackTime / TotalDuration;
	   	  }
	   	  ReplaySlider_var->SetValue(SliderValue);
	   }
	}
}
