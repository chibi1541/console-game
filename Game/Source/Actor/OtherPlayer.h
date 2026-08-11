#pragma once
#include "ReplicatedActor.h"

class OtherPlayer : public ReplicatedActor
{
	TYPE_DECLARATIONS(OtherPlayer, ReplicatedActor)
public:
	OtherPlayer(const Craft::Vector2& position, uint64 objectId);

	int64 GetPlayerId() const { return _playerId; }

private:
	int64 _playerId = 0;
};

