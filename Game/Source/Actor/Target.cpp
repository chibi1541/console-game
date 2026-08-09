#include "pch.h"
#include "Target.h"

using namespace Craft;

Target::Target(const Vector2& position)
	: super(L"T", position, Color::Blue)
{
	// 박스랑 플레이어 보다는 낮지만 바닥보다는 위로 오도록 설정
	sortingOrder = 3;
}
