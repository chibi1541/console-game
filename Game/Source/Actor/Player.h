#pragma once
#include "ReplicatedActor.h"
#include "Protocol/Enum.pb.h"

class SubActor;

struct Trail
{
	Craft::Vector2 pos = Craft::Vector2::Zero;
	Protocol::DirectionType prevDir = Protocol::DirectionType::DIR_NONE;
	Protocol::DirectionType curDir = Protocol::DirectionType::DIR_NONE;
};

class Player : public ReplicatedActor
{
	TYPE_DECLARATIONS(Player, ReplicatedActor)

public:
	enum AxisType
	{
		X = 0,
		Y = 1,
		NUMBER = 3,
	};

	Player(const Craft::Vector2& position, uint64 objectId);
	Player(const Craft::Vector2& position, Craft::Color color, uint64 objectId);

	float GetMoveSpeed() const { return moveSpeed; }
	void SetMoveSpeed(float speed) { moveSpeed = speed; }
	Protocol::DirectionType GetSyncDirectionType() { return syncDir; }
	void SetSyncDirection(Protocol::DirectionType direction) { syncDir = direction; }

	void UpdateTrailInfo(const google::protobuf::RepeatedPtrField<Protocol::TrailData>& trails);
	void UpdateNextTrailInfo(const google::protobuf::RepeatedPtrField<Protocol::TrailData>& trails);

	void UpdateSubActorPos();

	bool WarningTrailPos();

	virtual void InterpolateSync(float deltaTime);

	const Trail GetNextTrail(Craft::Vector2& pos) const;

	virtual void DestroyPlayer();

protected:
	virtual void Tick(float deltaTime) override;

protected:
	float						moveSpeed = 0.f;
	Protocol::DirectionType		syncDir;

	vector<std::wstring>		images;

	uint32						trailIndex = 0;
	deque<Trail>				trailQueue;
	vector<SubActorRef>			subActors;

	deque<Trail>				nextTrails;
	uint32						nextTrailIndex = 0;
private:
	

};