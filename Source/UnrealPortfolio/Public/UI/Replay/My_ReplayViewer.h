// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUI_PageBase.h"
// UE UI
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Sizebox.h"
#include "Components/Slider.h"
#include "My_ReplayViewer.generated.h"


/**
 * Replay Viewer
 * 1. Download Files from WebServer
 * 2. Tied files to one folder
 * 3. Init Datas
 * 4. Play Replay files
 * 5. additional function : stop, pause, play
 */
UCLASS()
class UNREALPORTFOLIO_API UMy_ReplayViewer : public UMyUI_PageBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UButton* Sound_Btn_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))   USlider* ReplaySlider_var;
	//class UImportedSoundWave* ImportedSoundWave;
	class USoundWave* ImportedSoundWave;

private:
	bool bIsPause = false;

	// playing control
	float CurrentPlaybackTime;
	float TotalDuration;
	float SliderValue;

public:
	UFUNCTION() void OnSliderValueChanged(float Value);
	
protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
