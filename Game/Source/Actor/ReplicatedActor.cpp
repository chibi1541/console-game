#include "pch.h"
#include "ReplicatedActor.h"

using namespace Craft;

ReplicatedActor::ReplicatedActor(const Craft::Vector2& position, uint64 objectId)
	:objectId(objectId)
{

}

ReplicatedActor::ReplicatedActor(const std::wstring& image, const Craft::Vector2& position, Craft::Color color, uint64 objectId)
	: Actor(image, position, color), objectId(objectId)
{
}

void ReplicatedActor::SetObjectId(uint64 newObjId)
{
	objectId = newObjId;
}


