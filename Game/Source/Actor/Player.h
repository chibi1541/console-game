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

	void SetSubActorsPrevSync(uint64 syncTickCount, const google::protobuf::RepeatedPtrField<Protocol::ActorInfo>& actorInfos);
	void SetSubActorsNextSync(uint64 syncTickCount, const google::protobuf::RepeatedPtrField<Protocol::ActorInfo>& actorInfos);

private:
	virtual void Tick(float deltaTime) override;

private:
	float _moveSpeed = 0.f;
	Protocol::DirectionType _syncDir;
	Protocol::DirectionType _localDir;

	vector<weak_ptr<SubActor>> _subActors;
	uint32 _tailIndex = 0;
};

