#include "pch.h"
#include "ReplicatedActor.h"
#include "Globals.h"

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

void ReplicatedActor::SetPrevSyncPos(const Craft::Vector2& pos)
{
	prevPos = pos;
	SetPosition(Vector2(prevPos.x/100, prevPos.y/100));
}

void ReplicatedActor::Tick(float deltaTime)
{
	
}


