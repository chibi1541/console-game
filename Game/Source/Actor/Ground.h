#pragma once
#include "Actor/Actor.h"
#include "Math/Vector2.h"

class Ground : public Craft::Actor
{
	TYPE_DECLARATIONS(Ground, Actor)

public:
	Ground(const Craft::Vector2& position);

};


