#include "pch.h"
#include "ServerPacketHandler.h"
#include "Engine/Engine.h"
#include "Level/TestLevel.h"
#include "ServerCore/Session.h"
#include "Actor/Player.h"
#include "Actor/OtherPlayer.h"
#include "Globals.h"
#include "Level/ReplicatedLevel.h"

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

	// TODO : 플레이어 정보 저장

	const Protocol::ActorInfo& actor = pkt.player().actor();

	Vector2 spawnPos = Vector2(actor.pos().x(), actor.pos().y());
	curLevel->SpawnActor<Player>(spawnPos, actor.objectid());

	return true;
}

bool Handle_S_SPAWN_ACTOR(PacketSessionRef& session, Protocol::S_SPAWN_ACTOR& pkt)
{
	if (userId != pkt.id())
	{
		std::shared_ptr<Level> curLevel = Engine::Get().GetLevel();
		ASSERT_CRASH(curLevel);

		Vector2 spawnPos = Vector2(pkt.spawnpos().x(), pkt.spawnpos().y());
		curLevel->SpawnActor<OtherPlayer>(spawnPos, pkt.id());
	}

	return true;
}

bool Handle_S_UPDATE_ROOM(PacketSessionRef& session, Protocol::S_UPDATE_ROOM& pkt)
{
	std::shared_ptr<Level> curLevel = Engine::Get().GetLevel();
	ASSERT_CRASH(curLevel);

	std::shared_ptr<ReplicatedLevel> rLevel = Cast<ReplicatedLevel>(curLevel);
	ASSERT_CRASH(rLevel);

	JobRef job = std::make_shared<Job>(rLevel, &ReplicatedLevel::UpateLevelReplicated, std::move(pkt));
	rLevel->Push(job);

	return true;
}
