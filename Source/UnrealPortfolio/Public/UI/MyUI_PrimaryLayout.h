// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUI_UserWidget.h"
#include "Components/SizeBox.h"
#include "UI/MyUI_PageBase.h"
#include "MyUI_PrimaryLayout.generated.h"

UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogPrimaryLayout, Log, All);
/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UMyUI_PrimaryLayout : public UMyUI_UserWidget
{
	GENERATED_BODY()
	
	// Various
public:
	UPROPERTY(meta = (BindWidget))	USizeBox* Game_Layer;
	UPROPERTY(meta = (BindWidget))	USizeBox* Menu_Layer;
	UPROPERTY(meta = (BindWidget))	USizeBox* Popup_Layer;
	UPROPERTY()	TArray<UMyUI_PageBase*> Popup_Stack;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;
	
	bool Close_Popup(UMyUI_PageBase* InWidget);
	UMyUI_PageBase* Open_Page(const FName& InName, bool InPopup, APlayerController* InPC, UUserWidget* InConnection);
	
};
