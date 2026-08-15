#pragma once
#include "Actor/Actor.h"
#include "Protocol/Enum.pb.h"


class SubActor : public Craft::Actor
{
	TYPE_DECLARATIONS(SubActor, Craft::Actor);

public:
	SubActor(const Craft::Vector2& position);
	SubActor(const Craft::Vector2& position, const Protocol::DirectionType& curDir, const Protocol::DirectionType& prevDir);
	//SubActor(const std::wstring& image, const Craft::Vector2& position, Craft::Color color, uint64 objectId);

	const Protocol::DirectionType& GetCurDir() const { return _curDir; }
	void SetCurDir(const Protocol::DirectionType& dirType) { _curDir  = dirType; }
	const Protocol::DirectionType& GetPrevDir() const {return _prevDir;}
	void SetPrevDir(const Protocol::DirectionType& dirType) { _prevDir = dirType; }

private:
	Protocol::DirectionType _curDir = Protocol::DirectionType::DIR_NONE;
	Protocol::DirectionType _prevDir = Protocol::DirectionType::DIR_NONE;
};

