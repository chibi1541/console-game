#include "pch.h"
#include "RemotePlayer.h"

RemotePlayer::RemotePlayer(const Craft::Vector2& position, uint64 objectId)
	: super(position, Craft::Color::Red, objectId)
{
	
}

RemotePlayer::RemotePlayer(const Craft::Vector2& position, Craft::Color color, uint64 objectId)
	: super(position, color, objectId)
{
	
}
