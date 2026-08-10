#include "pch.h"
#include "Player.h"
#include "Input/Input.h"

using namespace Craft;

Player::Player(const Vector2& position)
	: super(L"◑", position, Craft::Color::Green)
{
	sortingOrder = 10;
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

	// 지금 상태면 대각선 이동돼서 박스 밀리는 판단이 애매해지지 않나?
	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x += 1;

		SetPosition(newPosition);
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x -= 1;

		SetPosition(newPosition);
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y -= 1;

		SetPosition(newPosition);
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y += 1;

		SetPosition(newPosition);
	}
}
