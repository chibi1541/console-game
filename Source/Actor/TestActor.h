#pragma once

#include "Actor/Actor.h"

class TestActor : public Craft::Actor
{
public:
	TestActor();
	virtual void Tick(float deltaTime) override;

};

