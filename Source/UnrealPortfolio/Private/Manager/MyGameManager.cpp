// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/MyGameManager.h"
// Common
#include "Common/MyConfig.h"


UMyGameManager::UMyGameManager()
{
}

void UMyGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	// InitializeDependency �� ����Ͽ� GameManager ���� ������ �ش� �����ν��Ͻ����� ����
	Collection.InitializeDependency(UMyConfig::StaticClass());
	// ����
	// ���̺�
	// Http �� ���� ���� �������� �ϴ� �͵�

	Super::Initialize(Collection);

	// SaveData ��� ���� �� �ε�(�����)

	// ���Ϸ� ����� local profile ������ �о� ���δ�. 

	// Network Session Task , OnlineSystem ����

	// TrainingManager �� ����(Uobject) , NetworkMangager(GameInstanceSubSystem)�� ����

}

void UMyGameManager::Deinitialize()
{
}

void UMyGameManager::Init_Game(const FString& MapName, const FString& Options, FString& ErrorMessage, bool bGamemode)
{

}
