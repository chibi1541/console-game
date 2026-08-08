#include "pch.h"
#include "TestLevel.h"
#include "Actor/TestActor.h"


using namespace Craft;

void TestLevel::OnInitialized()
{
	Level::OnInitialized();

	// 테스트 액터 레벨에 추가
	SpawnActor<TestActor>();

}
