#include "pch.h"
#include "TestActor.h"
#include "Input/Input.h"
#include "Math/Vector2.h"
#include "Math/Color.h"

using namespace Craft;

TestActor::TestActor()
	: Actor("P", Vector2(5, 5), Color::Green)
{
	// 그리기 순서 값 설정.
	sortingOrder = 5;
}

void TestActor::Tick(float deltaTime)
{
	// 상위 로직 호출.
	Actor::Tick(deltaTime);

	// 프레임 관련 문자열
	const int size = 256;
	char fpsString[size] = {};
	sprintf_s(fpsString, size, "dt: %f | fps: %.1f", deltaTime, (1.f / deltaTime));

	// 콘솔 창 이름에 값 설정.
	SetConsoleTitleA(fpsString);

	// ESC 키 종료.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 엔진 종료.
		QuitGame();
	}

	// 방향키 이동 처리.
	// @Temp: 프레임시간 고려는 나중에.
	if (Input::Get().GetKey(VK_LEFT) && position.x > 0)
	{
		position.x -= 1;
	}

	if (Input::Get().GetKey(VK_RIGHT) && position.x < 100)
	{
		position.x += 1;
	}

	if (Input::Get().GetKey(VK_UP) && position.y > 0)
	{
		position.y -= 1;
	}

	if (Input::Get().GetKey(VK_DOWN) && position.y < 100)
	{
		position.y += 1;
	}
}