#include "pch.h"
#include "Player.h"
#include "Input/Input.h"
#include "Level/GameLevel.h"

using namespace Craft;

Player::Player(const Vector2& position)
	: super(L"", position, Color::Green)
{
	sortingOrder = 10;

	std::vector<BYTE> bytes = FileUtils::ReadFile(L"../Assets/warrior_walk_frame1.txt");
	string sour = string(bytes.begin(), bytes.end());
	image = FileUtils::Convert(sour);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 종료처리
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
		return;
	}

	std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());
	ASSERT_CRASH(level);

	// 지금 상태면 대각선 이동돼서 박스 밀리는 판단이 애매해지지 않나?
	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x += 1;

		if (level->CanMove(GetPosition(), newPosition))
			SetPosition(newPosition);
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x -= 1;

		if (level->CanMove(GetPosition(), newPosition))
			SetPosition(newPosition);
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y -= 1;

		if (level->CanMove(GetPosition(), newPosition))
			SetPosition(newPosition);
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y += 1;

		if (level->CanMove(GetPosition(), newPosition))
			SetPosition(newPosition);
	}
}
