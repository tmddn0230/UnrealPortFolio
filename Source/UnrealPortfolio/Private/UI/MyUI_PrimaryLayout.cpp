// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUI_PrimaryLayout.h"
// Framework
#include "Framework/MyPlayerController.h"
// Data
#include "Data/MyTableManager.h"
// Manager
#include "Manager/MyGameManager.h"

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

void UMyUI_PrimaryLayout::Inter_Change_PageWidget(UUserWidget* InWidget)
{
	if (Game_Layer && InWidget) {
		Game_Layer->ClearChildren();
		Game_Layer->AddChild(InWidget);
	}
}

void UMyUI_PrimaryLayout::Inter_Add_PopupWidget(UMyUI_PageBase* InWidget)
{
	if (InWidget && Popup_Layer) {
		Popup_Stack.Push(InWidget);
		Popup_Layer->AddChild(InWidget);
	}
}

void UMyUI_PrimaryLayout::Inter_Remove_PopupWidget(UUserWidget* InWidget)
{
	if (InWidget && Popup_Layer) {
		InWidget->RemoveFromParent();
		Popup_Layer->RemoveChild(InWidget);
	}
}

// Open Close
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
			if (auto* table = mgr->Get_TableManager()) {
				if (FMyTableRow_UI* table_row = table->Get_UI(InName)) {
					if (table_row && table_row->WidgetClass) {
						if (InPopup)
							return Inter_Open_Popup(InName, table_row->WidgetClass, InPC, InConnection);
						else
							return Inter_Open_Page(InName, table_row->WidgetClass, InPC, InConnection);
					}
				}
			}
		}
	}

	return nullptr;
}

UMyUI_PageBase* UMyUI_PrimaryLayout::Inter_Open_Page(const FName& InName, TSubclassOf<UUserWidget> UserWidgetClass, APlayerController* InPC, UUserWidget* InConnection)
{
	if (UUserWidget* widget = CreateWidget<UUserWidget>(InPC, UserWidgetClass)) {
		if (auto tempWidget = Cast<UMyUI_PageBase>(widget)) {
			if (InConnection) {
				tempWidget->Widget_Name = InName;
				tempWidget->Connection_Widget = InConnection;
			}
			Inter_Change_PageWidget(widget);
			return tempWidget;
		}
	}
	return nullptr;
}

UMyUI_PageBase* UMyUI_PrimaryLayout::Inter_Open_Popup(const FName& InName, TSubclassOf<UUserWidget> UserWidgetClass, APlayerController* InPC, UUserWidget* InConnection)
{
	if (UUserWidget* widget = CreateWidget<UUserWidget>(InPC, UserWidgetClass)) {
		if (auto tempWidget = Cast<UMyUI_PageBase>(widget)) {
			if (InConnection) {
				tempWidget->Widget_Name = InName;
				tempWidget->Connection_Widget = InConnection;
			}
			Inter_Add_PopupWidget(tempWidget);
			return tempWidget;
		}
	}
	return nullptr;
}
