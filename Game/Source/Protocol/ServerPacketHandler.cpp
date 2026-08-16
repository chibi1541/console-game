#include "pch.h"
#include "ServerPacketHandler.h"
#include "Engine/Engine.h"
#include "Level/TestLevel.h"
#include "ServerCore/Session.h"
#include "Actor/Player.h"
#include "Actor/OtherPlayer.h"
#include "Globals.h"
#include "Level/ReplicatedLevel.h"
#include "Utils/ObjectIdHandler.h"
#include "Actor/Item.h"
#include "Actor/SubActor.h"
#include "Manager/GameState.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (false == pkt.success())
		return false;

	std::shared_ptr<ReplicatedLevel> level = Cast<ReplicatedLevel>(Engine::Get().GetLevel());
	ASSERT_CRASH(level);

	Client::PlayerInfo localPlayer;
	localPlayer.userId = pkt.user().id();
	localPlayer.name = pkt.user().name();

	JobRef job = std::make_shared<Job>(level, &ReplicatedLevel::SetLocalPlayer, std::move(localPlayer));
	level->Push(job);

	Protocol::C_ENTER_GAME enterPkt;
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterPkt);
	session->Send(sendBuffer);
	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (false == pkt.success())
		return false;

	std::shared_ptr<ReplicatedLevel> level = Cast<ReplicatedLevel>(Engine::Get().GetLevel());
	ASSERT_CRASH(level);

	{
		JobRef job = std::make_shared<Job>(level, &ReplicatedLevel::InitField, pkt.width(), pkt.height());
		level->Push(job);
	}
	{
		vector<Protocol::PlayerInfo> players;
		players.reserve(pkt.players().size());
		for(auto player : pkt.players())
		{
			players.emplace_back(player);
		}

		JobRef job = std::make_shared<Job>(level, &ReplicatedLevel::InitPlayers, std::move(players));
		level->Push(job);
	}

	return true;
}

bool Handle_S_SPAWN_ACTOR(PacketSessionRef& session, Protocol::S_SPAWN_ACTOR& pkt)
{
	std::shared_ptr<ReplicatedLevel> level = Cast<ReplicatedLevel>(Engine::Get().GetLevel());
	ASSERT_CRASH(level);

	Vector2 spawnPos = Vector2(pkt.spawnpos().x() / 100, pkt.spawnpos().y() / 100);
	uint64 objectId = pkt.id();
	Protocol::ObjectType objectType = ObjectIdHandler::GetObjectType(objectId);

	switch (objectType)
	{
	case Protocol::ObjectType::OBJECT_SNAKE_HEAD:
	{
		// 스폰 패킷으로 오는 뱀 머리는 다 리모트 플레이어
		level->SpawnActor<OtherPlayer>(spawnPos, pkt.id());
		break;
	}
	case Protocol::ObjectType::OBJECT_ITEM:
	{
		level->SpawnActor<Item>(spawnPos, pkt.id());
		break;
	}
	}

	return true;
}

bool Handle_S_UPDATE_ROOM(PacketSessionRef& session, Protocol::S_UPDATE_ROOM& pkt)
{
	std::shared_ptr<ReplicatedLevel> level = Cast<ReplicatedLevel>(Engine::Get().GetLevel());
	ASSERT_CRASH(level);

	uint64 syncTick = ::GetTickCount64();
	JobRef job = std::make_shared<Job>(level, &ReplicatedLevel::AddLevelSnapshot, std::move(syncTick), std::move(pkt));
	level->Push(job);

	return true;
}

bool Handle_S_DESTROY_ACTOR(PacketSessionRef& session, Protocol::S_DESTROY_ACTOR& pkt)
{
	ReplLevelRef level = Cast<ReplicatedLevel>(Engine::Get().GetLevel());
	ASSERT_CRASH(level);

	JobRef job = std::make_shared<Job>(level, &ReplicatedLevel::DestroyReplicatedActor, pkt.id());
	level->Push(job);

	return false;
}
