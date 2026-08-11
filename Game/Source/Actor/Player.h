#pragma once
#include "ReplicatedActor.h"

class Player : public ReplicatedActor
{
	TYPE_DECLARATIONS(Player, ReplicatedActor)
public:
	Player(const Craft::Vector2& position, uint64 objectId);

private:
	virtual void Tick(float deltaTime) override;

private:
};

