// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/My_MessageBox.h"

// Framework
#include "Framework/MyPlayerController.h"
// UI
#include "UI/MyUI_Button.h"

FEventReply UMy_MessageBox::OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent)
{
	return FEventReply();
}

void UMy_MessageBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();
    // 위젯 주변을 클릭하면 Close 처리, 팝업창 주변을 누르면 팝업창이 사라지는 
	if (Tap_var) {
		Tap_var->OnMouseButtonDownEvent.BindUFunction(this, "OnClicked_TapToCloseZone");
	}
}

void UMy_MessageBox::Close_Dialog()
{
	if (auto* pc = Get_PlayerController()) {
		pc->Close_Popup(this);
	}
}

void UMy_MessageBox::Setup_Dialog(FTableRow_MessageBox* InInfo, FMyDele_MessageResult ResultCallback)
{
	Title_var->SetText(InInfo->Header);
	Description_var->SetText(InInfo->Body);

	Buttons_var->Reset<UButton>([](UButton& Button)
		{
			/////////////	Button.OnClicked().Clear();
		});

	for (const auto& ButtonInfo : InInfo->ButtonArray)
	{
		FDataTableRowHandle ActionRow;
		UMyUI_Button* ButtonWidget = Buttons_var->CreateEntry<UMyUI_Button>();

		switch (ButtonInfo.Result)
		{
		case EUP_MessageResult::Confirmed:
			break;
		case EUP_MessageResult::Declined:
			break;
		case EUP_MessageResult::Cancelled:
			if (ButtonWidget->ButtonWidget)
				ButtonWidget->Change_Texture(InInfo->Texture_Normal, InInfo->Texture_Hover, InInfo->Texture_Cancel);
			break;
		default:
			continue;
		}


		if (ButtonWidget->ButtonWidget) {
			ButtonWidget->Click_Delegate.BindUObject(this, &ThisClass::CloseConfirmationWindow);
			ButtonWidget->Click_Result = ButtonInfo.Result;
			ButtonWidget->Change_Text(ButtonInfo.Text);

		}
	}
	OnResultCallback = ResultCallback;
}

void UMy_MessageBox::CloseConfirmationWindow(EUP_MessageResult Result)
{
	Deactivate_Widget();
	OnResultCallback.ExecuteIfBound(Result);
}
