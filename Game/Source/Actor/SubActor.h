#pragma once
#include "ReplicatedActor.h"

class SubActor : public ReplicatedActor
{
	TYPE_DECLARATIONS(SubActor, ReplicatedActor);

public:
	SubActor(const Craft::Vector2& position, uint64 objectId);
	SubActor(const std::wstring& image, const Craft::Vector2& position, Craft::Color color, uint64 objectId);

private:
	virtual void Tick(float deltaTime) override;

};

