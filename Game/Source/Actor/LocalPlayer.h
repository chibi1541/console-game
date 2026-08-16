#pragma once
#include "Actor/Player.h"

class LocalPlayer : public Player
{
	TYPE_DECLARATIONS(LocalPlayer, Player)

public:
	LocalPlayer(const Craft::Vector2& position, uint64 objectId);
	LocalPlayer(const Craft::Vector2& position, Craft::Color color, uint64 objectId);
	

private:
	virtual void Tick(float deltaTime) override;

};

