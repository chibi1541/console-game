#pragma once
#include "Protocol/Protocol.pb.h"

class ReplicatedActor : public Craft::Actor
{
	TYPE_DECLARATIONS(Actor, ReplicatedActor)

public:
	ReplicatedActor(const Craft::Vector2& position, uint64 objectId);
	ReplicatedActor(const std::wstring& image ,const Craft::Vector2& position , Craft::Color color, uint64 objectId);

	uint64 GetObjectId() const { return objectId; }

	void SetObjectId(uint64 newObjId);


protected:
	uint64 objectId = 0;


};

