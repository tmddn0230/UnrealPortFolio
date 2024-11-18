// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUI_PrimaryLayout.h"
// Framework
#include "Framework/MyPlayerController.h"


DEFINE_LOG_CATEGORY(LogPrimaryLayout);

void UMyUI_PrimaryLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMyUI_PrimaryLayout::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UMyUI_PrimaryLayout::Initialize()
{
	bool res = Super::Initialize();
	return res;
}

void UMyUI_PrimaryLayout::BeginDestroy()
{
	Super::BeginDestroy();
}

bool UMyUI_PrimaryLayout::Close_Popup(UMyUI_PageBase* InWidget)
{
	bool res = false;

	auto* last = Popup_Stack.Last();
	if (last) {
		if (last == InWidget) {
			last->RemoveFromParent();
			Popup_Stack.Pop();
		}
		else {
			//check(0);
			return false;
		}
	}
	return res;
}

UMyUI_PageBase* UMyUI_PrimaryLayout::Open_Page(const FName& InName, bool InPopup, APlayerController* InPC, UUserWidget* InConnection)
{
	UE_LOG(LogPrimaryLayout, Log, TEXT("Open_PopupUI: %s"), *InName.ToString());

	if (AMyPlayerController* PC = Cast<AMyPlayerController>(InPC)) {
		if (auto* mgr = GetGameManager()) {
			//if (auto* table = mgr->Get_TableManager()) {
			//	if (FRsTableRow_UI* table_row = table->Get_UI(InName)) {
			//		if (table_row && table_row->WidgetClass) {
			//			if (InPopup)
			//				return Inter_Open_Popup(InName, table_row->WidgetClass, InPC, InConnection);
			//			else
			//				return Inter_Open_Page(InName, table_row->WidgetClass, InPC, InConnection);
			//		}
			//	}
			//}
		}
	}
	return 0;
}
