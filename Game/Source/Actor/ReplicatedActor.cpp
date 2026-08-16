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

void ReplicatedActor::SetPrevSyncPos(const Craft::Vector2& prevPos)
{
	_prevPos = prevPos;
	SetPosition(Vector2(_prevPos.x/100, _prevPos.y/100));
}

void ReplicatedActor::SetPosition(const Craft::Vector2& newPosition)
{
	// TODO : 보정 값 서버로부터 전달 받도록 게임 모드 쪽에서 동기화
	//Craft::Vector2 pos = newPosition;
	//pos.x = pos.x / 100;
	//pos.y = pos.y / 100;

	Actor::SetPosition(newPosition);
}

void ReplicatedActor::Tick(float deltaTime)
{
	
}


