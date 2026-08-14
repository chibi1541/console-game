#include "pch.h"
#include "SubActor.h"
#include "Globals.h"

SubActor::SubActor(const Craft::Vector2& position, uint64 objectId)
	: super(L"●", position, Craft::Color::Green, objectId)
{
	sortingOrder = 9;
}
