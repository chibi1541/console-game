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

	std::shared_ptr<ReplicatedLevel> level = Cast<ReplicatedLevel>(Engine::Get().GetLevel());
	ASSERT_CRASH(level);

	// TODO : 플레이어 정보 저장


	const Protocol::HeadData& head = pkt.player().head();
	userObjectId = head.actor().objectid();
	Vector2 spawnPos = Vector2(head.actor().pos().x() / 100, head.actor().pos().y() / 100);
	shared_ptr<Player> player = level->SpawnActor<Player>(spawnPos, head.actor().objectid());
	player->SetMoveSpeed(head.movespeed());
	player->SetDirection(head.dir());

	return true;
}

bool Handle_S_SPAWN_ACTOR(PacketSessionRef& session, Protocol::S_SPAWN_ACTOR& pkt)
{
	if (userObjectId != pkt.id())
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
