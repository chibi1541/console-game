#include "pch.h"
#include "SubActor.h"
#include "Globals.h"
#include "Protocol/Enum.pb.h"
#include "Player.h"

SubActor::SubActor(const Craft::Vector2& position)
	: super(L"═", position, Craft::Color::Green)
{
	sortingOrder = 9;

	// x축
	_images.emplace_back(L"═");
	// y축
	_images.emplace_back(L"║");
	_images.emplace_back(L"╝");
	_images.emplace_back(L"╔");
	_images.emplace_back(L"╚");
	_images.emplace_back(L"╗");
}

SubActor::SubActor(const Craft::Vector2& position, const Protocol::DirectionType& curDir, const Protocol::DirectionType& prevDir)
	: super(L"═", position, Craft::Color::Green), _curDir(curDir), _prevDir(prevDir)
{
	sortingOrder = 9;

	// x축
	_images.emplace_back(L"═");
	// y축
	_images.emplace_back(L"║");
	_images.emplace_back(L"╝");
	_images.emplace_back(L"╔");
	_images.emplace_back(L"╚");
	_images.emplace_back(L"╗");
}

void SubActor::Tick(float deltaTime)
{
	if(	_prevDir == Protocol::DirectionType::DIR_NONE ||
		_curDir == _prevDir)
	{
		int32 axis = static_cast<int32>(_curDir) / static_cast<int32>(Player::AxisType::NUMBER);

		image = _images[axis];
	}
	else
	{
		int32 curAxis = static_cast<int32>(_curDir) / static_cast<int32>(Player::AxisType::NUMBER);
		int32 prevAxis = static_cast<int32>(_prevDir) / static_cast<int32>(Player::AxisType::NUMBER);

		int32 yValue = (curAxis != 0) ? static_cast<int32>(_curDir) : static_cast<int32>(_prevDir);
		int32 xValue = (curAxis == 0) ? static_cast<int32>(_curDir) : static_cast<int32>(_prevDir);

		// 시계 방향
		if (((curAxis == 0) && (yValue - xValue != 2)) ||
			((curAxis == 1) && (yValue - xValue == 2)))
		{
			// prev			cur
			// DIR_DOWN -> 	DIR_LEFT 2
			// DIR_UP -> 	DIR_RIGHT 3
			// DIR_LEFT	-> 	DIR_UP 4
			// DIR_RIGHT -> DIR_DOWN 5

			int32 value = static_cast<int32>(_curDir) + 1;
			image = _images[value];
		}
		// 반시계 방향
		else
		{
			// DIR_UP	-> 	DIR_LEFT 2   
			// DIR_RIGHT -> DIR_UP	3   
			// DIR_DOWN -> 	DIR_RIGHT 3  
			// DIR_LEFT -> 	DIR_DOWN 4  

			int32 value = static_cast<int32>(_prevDir);
			int32 sign = static_cast<int32>(_prevDir % 2);

			value = (sign == 0) ? value : value + 2;
			image = _images[value];
		}
	}
}
