#include "pch.h"
#include "Wall.h"

using namespace Craft;

Wall::Wall(const Vector2& position)
	:Actor("#", position, Color::White)
{
	// 그리기 우선 순위 지정
	sortingOrder = 0;
}
