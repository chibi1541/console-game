#pragma once
#include "ReplicatedActor.h"
#include "Protocol/Enum.pb.h"

class SubActor;

class Player : public ReplicatedActor
{
	TYPE_DECLARATIONS(Player, ReplicatedActor)
public:
	Player(const Craft::Vector2& position, uint64 objectId);

	float GetMoveSpeed() const { return _moveSpeed; }
	void SetMoveSpeed(float speed) { _moveSpeed = speed; }
	Protocol::DirectionType GetSyncDirectionType() { return _syncDir; }
	void SetSyncDirection(Protocol::DirectionType direction) { _syncDir = direction; }

	//void SetSubActorsPrevSync(uint64 syncTickCount, const google::protobuf::RepeatedPtrField<Protocol::ActorInfo>& actorInfos);
	//void SetSubActorsNextSync(uint64 syncTickCount, const google::protobuf::RepeatedPtrField<Protocol::ActorInfo>& actorInfos);

	void UpdateTrailInfo(const google::protobuf::RepeatedPtrField<Protocol::Vector2>& trails);

	void UpdateSubActorPos();

	bool WarningTrailPos();

private:
	virtual void Tick(float deltaTime) override;

private:
	float _moveSpeed = 0.f;
	Protocol::DirectionType _syncDir;
	Protocol::DirectionType _localDir;

	uint32					_trailIndex = 0;
	deque<Craft::Vector2>	_trailQueue;
	vector<SubActorRef>		_subActors;

};