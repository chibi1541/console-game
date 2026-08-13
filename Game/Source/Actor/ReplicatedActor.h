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

	void SetPrevSyncTick(uint64 tickCount) { _prevSyncTick = tickCount; }
	int64 GetPrevSyncTick() const { return _prevSyncTick; }

	void SetPrevSyncPos(const Craft::Vector2& prevPos) { _prevPos = prevPos; }

	void SetNextSyncTick(uint64 tickCount) { _nextSyncTick = tickCount; }
	int64 GetNextSyncTick() const { return _nextSyncTick; }

	void SetNextSyncPos(const Craft::Vector2& nextPos) { _nextPos = nextPos; }

	virtual void SetPosition(const Craft::Vector2& newPosition) override;

protected:
	virtual void Tick(float deltaTime) override;

protected:
	uint64 objectId = 0;

	uint64 _nextSyncTick = 0;
	uint64 _prevSyncTick = 0;

	Craft::Vector2 _nextPos;
	Craft::Vector2 _prevPos;

	int32 calcXPos = 0;
	int32 calcYPos = 0;
};

