#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2.h"

class Wall : public Craft::Actor
{
	TYPE_DECLARATIONS(Wall, Actor)

public:
	Wall(const Craft::Vector2& position);

};

