#pragma once
#include "Actor/Actor.h"


class SubActor : public Craft::Actor
{
	TYPE_DECLARATIONS(SubActor, Craft::Actor);

public:
	SubActor(const Craft::Vector2& position);
	//SubActor(const std::wstring& image, const Craft::Vector2& position, Craft::Color color, uint64 objectId);

private:

};

