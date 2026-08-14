#include "pch.h"
#include "SubActor.h"
#include "Globals.h"

SubActor::SubActor(const Craft::Vector2& position)
	: super(L"●", position, Craft::Color::Green)
{
	sortingOrder = 9;
}
