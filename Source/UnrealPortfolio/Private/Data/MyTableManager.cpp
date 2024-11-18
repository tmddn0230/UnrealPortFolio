// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MyTableManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Data/MyTableRows.h"

#define UPDEF_TABLE_PATH "/Game/UP1/Table/"
#define UPDEF_LOAD_TABLE(table_value,table_name) \
static ConstructorHelpers::FObjectFinder<UDataTable>Object_##table_value(TEXT(UPDEF_TABLE_PATH#table_name"."#table_name)); \
if (Object_##table_value.Succeeded() && Object_##table_value.Object != nullptr) {	\
	table_value = Object_##table_value.Object;	\
}

UMyTableManager::UMyTableManager()
{

}

void UMyTableManager::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UMyTableManager::Deinitialize()
{

}

FMyTableRow_UI* UMyTableManager::Get_UI(const FName& row_name)
{
	FMyTableRow_UI* OutRow = Widget_Table->FindRow<FMyTableRow_UI>(row_name, "");
	return OutRow;
}

FTableRow_MessageBox* UMyTableManager::Get_MessageBox(const FName& row_name)
{
	FTableRow_MessageBox* OutRow = MessageBox_Table->FindRow<FTableRow_MessageBox>(row_name, "");
	return OutRow;
}

TSubclassOf<UUserWidget> UMyTableManager::Get_UI_Widget(const FName& row_name)
{
	if (FMyTableRow_UI* table_row = Get_UI(row_name)) {
		return table_row->WidgetClass;
	}
	return nullptr;
}
