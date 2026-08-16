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

	void SetPrevSyncTick(uint64 tickCount) { prevSyncTick = tickCount; }
	int64 GetPrevSyncTick() const { return prevSyncTick; }

	void SetPrevSyncPos(const Craft::Vector2& pos);

	void SetNextSyncTick(uint64 tickCount) { nextSyncTick = tickCount; }
	int64 GetNextSyncTick() const { return nextSyncTick; }

	void SetNextSyncPos(const Craft::Vector2& pos) { nextPos = pos; }

protected:
	virtual void Tick(float deltaTime) override;

protected:
	uint64 objectId = 0;

	uint64 nextSyncTick = 0;
	uint64 prevSyncTick = 0;

	Craft::Vector2 nextPos;
	Craft::Vector2 prevPos;

	int32 calcXPos = 0;
	int32 calcYPos = 0;
};

