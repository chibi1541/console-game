#include "pch.h"
#include "OtherPlayer.h"

using namespace Craft;

OtherPlayer::OtherPlayer(const Vector2& position, uint64 objectId)
	: super(L"◐", position, Craft::Color::Red, objectId)
{
	sortingOrder = 10;
}
