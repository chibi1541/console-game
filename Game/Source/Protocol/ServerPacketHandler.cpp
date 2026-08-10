#include "pch.h"
#include "ServerPacketHandler.h"
#include "Engine/Engine.h"
#include "Level/TestLevel.h"
#include "ServerCore/Session.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	ASSERT_CRASH(pkt.success());

	Craft::Engine engine;
	engine.AddNewLevel<TestLevel>();
	engine.Run();

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	return true;
}
