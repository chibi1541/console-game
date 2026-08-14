#pragma once
#include "ReplicatedActor.h"

class Item : public ReplicatedActor
{
	TYPE_DECLARATIONS(Item, ReplicatedActor)

public:
	Item(const Craft::Vector2& position, uint64 objectId);

private:

};

