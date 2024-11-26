// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUI_PageBase.h"
// UE
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Components/Button.h"
// TableRows
#include "Data/MyTableRows.h"
// Common
#include "Common/MyCommon.h"

#include "My_MessageBox.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UMy_MessageBox : public UMyUI_PageBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(meta = (BindWidget))
	UBorder* Tap_var;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Title_var;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<URichTextBlock> Description_var;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UDynamicEntryBox> Buttons_var;

public:
	FMyDele_MessageResult OnResultCallback;

protected:
	FEventReply  OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent);
	virtual void NativeOnInitialized() override;
	void         Close_Dialog();
public:
	void         Setup_Dialog(FTableRow_MessageBox* InInfo, FMyDele_MessageResult ResultCallback);
	

// Binding
	void CloseConfirmationWindow(EUP_MessageResult Result);

};
