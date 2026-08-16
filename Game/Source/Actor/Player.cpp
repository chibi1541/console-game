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
	: super(L"►", position, Craft::Color::Green, objectId)
{
	sortingOrder = 10;

	images.emplace_back(L"◄");
	images.emplace_back(L"►");
	images.emplace_back(L"▲");
	images.emplace_back(L"▼");
}

Player::Player(const Craft::Vector2& position, Craft::Color color, uint64 objectId)
	: super(L"►", position, color, objectId)
{
	sortingOrder = 10;

	images.emplace_back(L"◄");
	images.emplace_back(L"►");
	images.emplace_back(L"▲");
	images.emplace_back(L"▼");
}

void Player::UpdateTrailInfo(const google::protobuf::RepeatedPtrField<Protocol::TrailData>& trails)
{
	trailQueue.clear();
	for (const Protocol::TrailData& newTrail : trails)
	{
		Trail trail;
		trail.pos = Vector2(newTrail.pos().x(), newTrail.pos().y());
		trail.prevDir = newTrail.prevdir();
		trail.curDir = newTrail.curdir();

		trailQueue.emplace_back(trail);
	}

	trailIndex = static_cast<int32>(trailQueue.size());

	shared_ptr<Level> level = owner.lock();
	ASSERT_CRASH(level);

	while (trailIndex > subActors.size())
	{
		SubActorRef subActor = level->SpawnActor<SubActor>(Vector2::Zero, color);
		subActors.emplace_back(subActor);
	}

	UpdateSubActorPos();
}

void Player::UpdateNextTrailInfo(const google::protobuf::RepeatedPtrField<Protocol::TrailData>& trails)
{
	nextTrails.clear();
	for (const Protocol::TrailData& newTrail : trails)
	{
		Trail trail;
		trail.pos = Vector2(newTrail.pos().x(), newTrail.pos().y());
		trail.prevDir = newTrail.prevdir();
		trail.curDir = newTrail.curdir();

		nextTrails.emplace_back(trail);
	}

	nextTrailIndex = static_cast<int32>(nextTrails.size());
}

void Player::UpdateSubActorPos()
{
	ASSERT_CRASH(trailIndex == subActors.size());

	for (uint32 idx = 0; idx < trailIndex; ++idx)
	{
		subActors[idx]->SetPosition(trailQueue[idx].pos);
		subActors[idx]->SetCurDir(trailQueue[idx].curDir);
		subActors[idx]->SetPrevDir(trailQueue[idx].prevDir);
	}
}

bool Player::WarningTrailPos()
{
	int32 size = static_cast<int32>(trailQueue.size());

	for (int32 i = 0;i < size - 1; ++i)
	{
		for (int32 j = i + 1; j < size; ++j)
		{
			if (trailQueue[i].pos == trailQueue[j].pos)
				return true;
		}
	}

	return false;
}

void Player::InterpolateSync(float deltaTime)
{
	int64 b = nextSyncTick - prevSyncTick;

	float delta = (b > 0) ? static_cast<float>(GDelayedTickCount - prevSyncTick) / static_cast<float>(b) : deltaTime;

	AxisType axisType = static_cast<AxisType>(static_cast<int32>(syncDir) / static_cast<int32>(AxisType::NUMBER));

	if(axisType == AxisType::X)
	{
		calcXPos = static_cast<int32>((prevPos.x / 100) + ((nextPos.x / 100) - (prevPos.x / 100)) * delta);
		calcYPos = (trailQueue.size() > 0) ? trailQueue.back().pos.y : position.y;
	}
	else
	{
		calcYPos = static_cast<int32>((prevPos.y / 100) + ((nextPos.y / 100) - (prevPos.y / 100)) * delta);
		calcXPos = (trailQueue.size() > 0) ? trailQueue.back().pos.x : position.x;
	}

	Vector2 newPos = Vector2(calcXPos, calcYPos);

	// 좌표 이동이 발생했다면 다음 싱크 데이터의 궤적에서 방향 정보를 가지고 방향을 전환
	// 꽁무니 위치를 갱신
	if(newPos != position)
	{
		Trail nextTrail = GetNextTrail(newPos);
		if(nextTrail.curDir != Protocol::DirectionType::DIR_NONE)
			syncDir = nextTrail.curDir;

		const Trail newTrail = GetNextTrail(position);
		if(newTrail.curDir != Protocol::DirectionType::DIR_NONE)
		{
			trailQueue.emplace_back(newTrail);

			if (trailIndex < nextTrailIndex)
			{
				++trailIndex;

				shared_ptr<Level> level = owner.lock();
				ASSERT_CRASH(level);

				while (trailIndex > subActors.size())
				{
					SubActorRef subActor = level->SpawnActor<SubActor>(newTrail.pos, color, newTrail.curDir, newTrail.prevDir);
					subActors.emplace_back(subActor);
				}
			}
			else
				trailQueue.pop_front();

			UpdateSubActorPos();
		}
	}

	SetPosition(newPos);
}

const Trail Player::GetNextTrail(Craft::Vector2& pos) const
{
	for (int32 idx = nextTrailIndex - 1; idx >= 0; --idx)
	{
		if(nextTrails[idx].pos == pos)
		{
			return nextTrails[idx];
		}
	}

	return Trail();
}

void Player::DestroyPlayer()
{
	Destroy();

	for(SubActorRef& trail : subActors)
	{
		trail->Destroy();
	}

	subActors.clear();
}

void Player::Tick(float deltaTime)
{
	if (prevSyncTick == 0)
		return;

	// TODO : AI용 로직을 추가
	// 종료처리
	//if (Input::Get().GetKeyDown(VK_ESCAPE))
	//{
	//	QuitGame();
	//	return;
	//}

	//if (Input::Get().GetKeyDown(VK_RIGHT))
	//{
	//	Vector2 newPosition = GetPosition();
	//	newPosition.x += 1;

	//	// TODO : 클라에서 서버 쪽으로 패킷 보내는 로직도 별도의 분리가 필요
	//	Protocol::C_MOVE_ACTOR pkt;
	//	pkt.set_newdir(Protocol::DirectionType::DIR_RIGHT);

	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	//	GService.get()->Broadcast(sendBuffer);
	//}

	//if (Input::Get().GetKeyDown(VK_LEFT))
	//{
	//	Vector2 newPosition = GetPosition();
	//	newPosition.x -= 1;

	//	Protocol::C_MOVE_ACTOR pkt;
	//	pkt.set_newdir(Protocol::DirectionType::DIR_LEFT);

	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	//	GService.get()->Broadcast(sendBuffer);
	//}

	//if (Input::Get().GetKeyDown(VK_UP))
	//{
	//	Vector2 newPosition = GetPosition();
	//	newPosition.y -= 1;

	//	Protocol::C_MOVE_ACTOR pkt;
	//	pkt.set_newdir(Protocol::DirectionType::DIR_UP);

	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	//	GService.get()->Broadcast(sendBuffer);
	//}

	//if (Input::Get().GetKeyDown(VK_DOWN))
	//{
	//	Vector2 newPosition = GetPosition();
	//	newPosition.y += 1;

	//	Protocol::C_MOVE_ACTOR pkt;
	//	pkt.set_newdir(Protocol::DirectionType::DIR_DOWN);

	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	//	GService.get()->Broadcast(sendBuffer);
	//}

	Vector2 prevPos = GetPosition();

	InterpolateSync(deltaTime);

	if(syncDir != Protocol::DirectionType::DIR_NONE)
		image = images[static_cast<int32>(syncDir) - 1];

}
