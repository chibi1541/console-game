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

//void Player::SetSubActorsPrevSync(uint64 syncTickCount, const google::protobuf::RepeatedPtrField<Protocol::ActorInfo>& actorInfos)
//{
//	for (auto actorInfo : actorInfos)
//	{
//		for (auto actor : _subActors)
//		{
//			auto actorRef = actor.lock();
//			if (actorRef && actorRef->GetObjectId() == actorInfo.objectid())
//			{
//				actorRef->SetPrevSyncTick(syncTickCount);
//				actorRef->SetPrevSyncPos(Craft::Vector2(actorInfo.pos().x(), actorInfo.pos().y()));
//				break;
//			}
//		}
//	}
//}
//
//void Player::SetSubActorsNextSync(uint64 syncTickCount, const google::protobuf::RepeatedPtrField<Protocol::ActorInfo>& actorInfos)
//{
//	for (auto actorInfo : actorInfos)
//	{
//		for (auto actor : _subActors)
//		{
//			auto actorRef = actor.lock();
//			if (actorRef && actorRef->GetObjectId() == actorInfo.objectid())
//			{
//				actorRef->SetNextSyncTick(syncTickCount);
//				actorRef->SetNextSyncPos(Craft::Vector2(actorInfo.pos().x(), actorInfo.pos().y()));
//				break;
//			}
//		}
//	}
//}

void Player::UpdateTrailInfo(const google::protobuf::RepeatedPtrField<Protocol::TrailData>& trails)
{
	_trailQueue.clear();
	for (const Protocol::TrailData& newTrail : trails)
	{
		Trail trail;
		trail.pos = Vector2(newTrail.pos().x(), newTrail.pos().y());
		trail.prevDir = newTrail.prevdir();
		trail.curDir = newTrail.curdir();

		_trailQueue.emplace_back(trail);
	}

	_trailIndex = static_cast<int32>(_trailQueue.size());

	shared_ptr<Level> level = owner.lock();
	ASSERT_CRASH(level);

	while (_trailIndex > _subActors.size())
	{
		SubActorRef subActor = level->SpawnActor<SubActor>(Vector2::Zero);
		_subActors.emplace_back(subActor);
	}

	UpdateSubActorPos();
}

void Player::UpdateSubActorPos()
{
	//ASSERT_CRASH(WarningTrailPos() == false);

	ASSERT_CRASH(_trailIndex == _subActors.size());

	for (uint32 idx = 0; idx < _trailIndex; ++idx)
	{
		_subActors[idx]->SetPosition(_trailQueue[idx].pos);
		_subActors[idx]->SetCurDir(_trailQueue[idx].curDir);
		_subActors[idx]->SetPrevDir(_trailQueue[idx].prevDir);
	}
}

bool Player::WarningTrailPos()
{
	int32 size = static_cast<int32>(_trailQueue.size());

	for (int32 i = 0;i < size - 1; ++i)
	{
		for (int32 j = i + 1; j < size; ++j)
		{
			if (_trailQueue[i].pos == _trailQueue[j].pos)
				return true;
		}
	}

	return false;
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

	Vector2 prevPos = GetPosition();



	super::Tick(deltaTime);

	// 궤적 보간
	// 이동 발생했다면 클라에서도 자체적으로 궤적을 갱신해줌
	//if (prevPos != position && _trailIndex != 0)
	//{
	//	for (int32 idx = 0; idx < _trailIndex; ++idx)
	//	{
	//		ASSERT_CRASH(position != _trailQueue[idx]);
	//	}

	//	int32 xDelta = position.x - prevPos.x;
	//	int32 yDelta = position.y - prevPos.y;

	//	// 좌표 값 사이의 부호 방향이 나옴
	//	int32 xValue = (xDelta != 0) ? (xDelta / ::abs(xDelta)) : 0;
	//	int32 yValue = (yDelta != 0) ? (yDelta / ::abs(yDelta)) : 0;

	//	// 2칸 이상 움직인 겨우 1 이상의 값이 나옴
	//	int32 xCount = ::abs(xDelta) - 1;
	//	int32 yCount = ::abs(yDelta) - 1;

	//	_trailQueue.emplace_back(prevPos);
	//	_trailQueue.pop_front();

	//	while (xCount > 0)
	//	{
	//		prevPos.x += xValue;

	//		_trailQueue.emplace_back(prevPos);
	//		_trailQueue.pop_front();
	//		
	//		--xCount;
	//	}

	//	while (yCount > 0)
	//	{
	//		prevPos.y += yValue;

	//		_trailQueue.emplace_back(prevPos);
	//		_trailQueue.pop_front();

	//		--yCount;
	//	}

	//	UpdateSubActorPos();
	//}

	// temp : 디버그용
	//{
	//	const std::wstring ypos = std::format(L"Prev : x{} y{} Cur : x{} y{}", prevPos.x, prevPos.y, position.x, position.y);
	//	Craft::Renderer::Get().Submit(ypos, Craft::Vector2(0, 2), Craft::Color::BrightWhite, 100);
	//}
}
