#pragma once
#include "ReplicatedActor.h"
#include "Protocol/Enum.pb.h"

class Player : public ReplicatedActor
{
	TYPE_DECLARATIONS(Player, ReplicatedActor)
public:
	Player(const Craft::Vector2& position, uint64 objectId);

	float GetMoveSpeed() const { return _moveSpeed; }
	void SetMoveSpeed(float speed) { _moveSpeed = speed; }
	Protocol::DirectionType GetDirectionType() { return _direction; }
	void SetDirection(Protocol::DirectionType direction) { _direction = direction; }

private:
	virtual void Tick(float deltaTime) override;

	virtual void SetPosition(const Craft::Vector2& newPosition) override;
private:
	float _moveSpeed = 0.f;
	Protocol::DirectionType _direction;
	float _xPos = 0;
	float _yPos = 0;
};

