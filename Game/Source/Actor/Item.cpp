#include "pch.h"
#include "Item.h"

Item::Item(const Craft::Vector2& position, uint64 objectId)
	: super(L"♣", position, Craft::Color::Blue, objectId)
{
	sortingOrder = 9;
}
