#pragma once
#include "Protocol/Enum.pb.h"


class ObjectIdHandler
{
	enum : uint64
	{
		OBJECT_TYPE_MASK = 0xFFFF'0000'0000'0000,
		OBJECT_COUNT_MASK = 0x0000'FFFF'FFFF'FFFF,
	};

public:
	static uint64 GenerateObjectId(Protocol::ObjectType type);

	static Protocol::ObjectType GetObjectType(uint64 objectId);
};

