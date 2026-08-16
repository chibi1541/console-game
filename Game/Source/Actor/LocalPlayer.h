#pragma once
#include "Actor/Player.h"

class LocalPlayer : public Player
{
	enum {WARNING_VALUE = 3,};

	TYPE_DECLARATIONS(LocalPlayer, Player)

public:
	LocalPlayer(const Craft::Vector2& position, uint64 objectId);
	LocalPlayer(const Craft::Vector2& position, Craft::Color color, uint64 objectId);
	

private:
	virtual void Tick(float deltaTime) override;

	void ProcessPlayerInput();
	void ProcessAI(float deltaTime);

	float _elapsedTime = 2.f;
};

