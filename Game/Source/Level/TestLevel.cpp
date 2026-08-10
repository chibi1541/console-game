#include "pch.h"
#include "TestLevel.h"
#include "Actor/Player.h"
#include "Engine/Engine.h"

using namespace Craft;

void TestLevel::OnInitialized()
{
	Level::OnInitialized();

	// 테스트 액터 레벨에 추가
	SpawnActor<Player>(Vector2(Engine::Get().GetWidth() / 2, Engine::Get().GetHeight() / 2));

}
