#include "pch.h"
#include "SubActor.h"
#include "Globals.h"
#include "Protocol/Enum.pb.h"

SubActor::SubActor(const Craft::Vector2& position)
	: super(L"●", position, Craft::Color::Green)
{
	sortingOrder = 9;
}

SubActor::SubActor(const Craft::Vector2& position, const Protocol::DirectionType& curDir, const Protocol::DirectionType& prevDir)
	: super(L"●", position, Craft::Color::Green), _curDir(curDir), _prevDir(prevDir)
{
	sortingOrder = 9;
}
