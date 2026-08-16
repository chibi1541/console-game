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

	float GetMoveSpeed() const { return _moveSpeed; }
	void SetMoveSpeed(float speed) { _moveSpeed = speed; }
	Protocol::DirectionType GetSyncDirectionType() { return _syncDir; }
	void SetSyncDirection(Protocol::DirectionType direction) { _syncDir = direction; }

	void UpdateTrailInfo(const google::protobuf::RepeatedPtrField<Protocol::TrailData>& trails);
	void UpdateNextTrailInfo(const google::protobuf::RepeatedPtrField<Protocol::TrailData>& trails);

	void UpdateSubActorPos();

	bool WarningTrailPos();

	virtual void InterpolateSync(float deltaTime);

	const Trail GetNextTrail(Craft::Vector2& pos) const;

private:
	virtual void Tick(float deltaTime) override;

private:
	float _moveSpeed = 0.f;
	Protocol::DirectionType _syncDir;
	Protocol::DirectionType _localDir;

	vector<std::wstring> _images;

	uint32					_trailIndex = 0;
	deque<Trail>			_trailQueue;
	vector<SubActorRef>		_subActors;

	deque<Trail>			_nextTrails;
	uint32					_nextTrailIndex = 0;

};