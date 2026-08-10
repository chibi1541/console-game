#include "pch.h"
#include "OtherPlayer.h"

OtherPlayer::OtherPlayer(const Craft::Vector2& position)
	: super(L"◐", position, Craft::Color::Red)
{
	sortingOrder = 10;
}
