#include "pch.h"
#include "Wall.h"

Wall::Wall(const Craft::Vector2& position)
	: super(L"@", position, Craft::Color::Red)
{
	// ⚡☀
	sortingOrder = 20;
}
