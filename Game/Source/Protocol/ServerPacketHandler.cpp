#include "pch.h"
#include "ServerPacketHandler.h"
#include "Engine/Engine.h"
#include "Level/TestLevel.h"
#include "ServerCore/Session.h"
#include "Actor/Player.h"
#include "Actor/OtherPlayer.h"
#include "Globals.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (false == pkt.success())
		return false;

	Protocol::C_ENTER_GAME enterPkt;
	
	userId = pkt.user().id();
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterPkt);
	session->Send(sendBuffer);
	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (false == pkt.success())
		return false;

	std::shared_ptr<Level> curLevel = Engine::Get().GetLevel();
	ASSERT_CRASH(curLevel);

	Vector2 spawnPos = Vector2(pkt.spawnpos().x(), pkt.spawnpos().y());
	curLevel->SpawnActor<Player>(spawnPos);

	int size = pkt.players_size();
	for (int i = 0;i < size; ++i)
	{
		const Protocol::PlayerInfo& player = pkt.players(i);

		Vector2 spawnPos = Vector2(player.pos().x(), player.pos().y());
		curLevel->SpawnActor<OtherPlayer>(spawnPos);
	}

	return true;
}

bool Handle_S_SPAWN_ACTOR(PacketSessionRef& session, Protocol::S_SPAWN_ACTOR& pkt)
{
	if (userId != pkt.id())
	{
		std::shared_ptr<Level> curLevel = Engine::Get().GetLevel();
		ASSERT_CRASH(curLevel);

		Vector2 spawnPos = Vector2(pkt.spawnpos().x(), pkt.spawnpos().y());
		curLevel->SpawnActor<OtherPlayer>(spawnPos);
	}

	return true;
}
