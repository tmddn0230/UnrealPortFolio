// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUI_UserWidget.h"
// UE
#include "Components/Button.h"
#include "Components/TextBlock.h"
// Common
#include "Common/MyCommon.h"

#include "MyUI_Button.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UMyUI_Button : public UMyUI_UserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	// Click Delegate
	DECLARE_DELEGATE_OneParam(FClickDelegate, EUP_MessageResult);

	FClickDelegate    Click_Delegate;
	EUP_MessageResult Click_Result = EUP_MessageResult::Unknown;

	UTextBlock* ButtonTextBlock;
	UButton*    ButtonWidget;
	FText       ButtonText;

public:

	UFUNCTION(BlueprintCallable)
	void OnClicked_Button();

	void Clear_ClickEvent();

	void Change_Text(const FText& InText);
	void Change_Texture(UObject* InTexture_N, UObject* InTexture_H, UObject* InTexture_C);
};
