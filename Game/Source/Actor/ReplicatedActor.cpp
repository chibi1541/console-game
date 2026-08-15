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

void ReplicatedActor::SetPosition(const Craft::Vector2& newPosition)
{
	// TODO : 보정 값 서버로부터 전달 받도록 게임 모드 쪽에서 동기화
	Craft::Vector2 pos = newPosition;
	pos.x = pos.x / 100;
	pos.y = pos.y / 100;

	Actor::SetPosition(pos);
}

void ReplicatedActor::Tick(float deltaTime)
{
	int64 b = _nextSyncTick - _prevSyncTick;

	float delta = (b > 0) ? (_nextSyncTick - GDelayedTickCount) / b : deltaTime;

	calcXPos = static_cast<int32>(_prevPos.x + (_nextPos.x - _prevPos.x) * delta);
	calcYPos = static_cast<int32>(_prevPos.y + (_nextPos.y - _prevPos.y) * delta);

	SetPosition(Vector2(calcXPos, calcYPos));
}


