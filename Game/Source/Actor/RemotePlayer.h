#pragma once
#include "Actor/Player.h"

class RemotePlayer : public Player
{
	TYPE_DECLARATIONS(RemotePlayer, Player)

public:
	RemotePlayer(const Craft::Vector2& position, uint64 objectId);
	RemotePlayer(const Craft::Vector2& position, Craft::Color color, uint64 objectId);


};

