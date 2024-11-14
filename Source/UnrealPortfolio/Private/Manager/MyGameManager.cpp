// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/MyGameManager.h"
// Common
#include "Common/MyConfig.h"


UMyGameManager::UMyGameManager()
{
}

void UMyGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	// InitializeDependency 을 사용하여 GameManager 보다 빠르게 해당 게임인스턴스들을 실행
	Collection.InitializeDependency(UMyConfig::StaticClass());
	// 세션
	// 테이블
	// Http 등 값이 먼저 정해져야 하는 것들

	Super::Initialize(Collection);

	// SaveData 경로 지정 및 로드(재시작)

	// 파일로 저장된 local profile 정보를 읽어 들인다. 

	// Network Session Task , OnlineSystem 사용시

	// TrainingManager 를 실행(Uobject) , NetworkMangager(GameInstanceSubSystem)를 통해

}

void UMyGameManager::Deinitialize()
{
}

void UMyGameManager::Init_Game(const FString& MapName, const FString& Options, FString& ErrorMessage, bool bGamemode)
{

}
