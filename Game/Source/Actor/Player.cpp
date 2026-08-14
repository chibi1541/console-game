#include "pch.h"
#include "Player.h"
#include "Input/Input.h"
#include <Protocol/ServerPacketHandler.h>
#include <Protocol/Protocol.pb.h>
#include <Globals.h>
#include <ServerCore/Service.h>
#include "Protocol/Enum.pb.h"
#include "Render/Renderer.h"
#include "Actor/SubActor.h"


using namespace Craft;

Player::Player(const Vector2& position, uint64 objectId)
	: super(L"◑", position, Craft::Color::Green, objectId)
{
	sortingOrder = 10;
}

void Player::SetSubActorsPrevSync(uint64 syncTickCount, const google::protobuf::RepeatedPtrField<Protocol::ActorInfo>& actorInfos)
{
	for (auto actorInfo : actorInfos)
	{
		for (auto actor : _subActors)
		{
			auto actorRef = actor.lock();
			if (actorRef && actorRef->GetObjectId() == actorInfo.objectid())
			{
				actorRef->SetPrevSyncTick(syncTickCount);
				actorRef->SetPrevSyncPos(Craft::Vector2(actorInfo.pos().x(), actorInfo.pos().y()));
				break;
			}
		}
	}
}

void Player::SetSubActorsNextSync(uint64 syncTickCount, const google::protobuf::RepeatedPtrField<Protocol::ActorInfo>& actorInfos)
{
	for (auto actorInfo : actorInfos)
	{
		for (auto actor : _subActors)
		{
			auto actorRef = actor.lock();
			if (actorRef && actorRef->GetObjectId() == actorInfo.objectid())
			{
				actorRef->SetNextSyncTick(syncTickCount);
				actorRef->SetNextSyncPos(Craft::Vector2(actorInfo.pos().x(), actorInfo.pos().y()));
				break;
			}
		}
	}
}

void Player::Tick(float deltaTime)
{
	if (_prevSyncTick == 0)
		return;

	// TODO : 입력 처리 따로 빼기

	// 종료처리
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
		return;
	}

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x += 1;

		// TODO : 클라에서 서버 쪽으로 패킷 보내는 로직도 별도의 분리가 필요
		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_RIGHT);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

		_localDir = Protocol::DirectionType::DIR_RIGHT;
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x -= 1;

		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_LEFT);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

		_localDir = Protocol::DirectionType::DIR_LEFT;
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y -= 1;

		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_UP);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

		_localDir = Protocol::DirectionType::DIR_UP;
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y += 1;

		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_DOWN);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

		_localDir = Protocol::DirectionType::DIR_DOWN;
	}

	super::Tick(deltaTime);

	// temp : 디버그용
	{
		const std::wstring ypos = std::format(L"Prev : {} Cur : {} Next : {}", _prevPos.y, calcYPos, _nextPos.y);
		Craft::Renderer::Get().Submit(ypos, Craft::Vector2(0, 2), Craft::Color::BrightWhite, 100);
	}
}
