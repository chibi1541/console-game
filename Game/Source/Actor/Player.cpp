#include "pch.h"
#include "Player.h"
#include "Input/Input.h"
#include <Protocol/ServerPacketHandler.h>
#include <Protocol/Protocol.pb.h>
#include <Globals.h>
#include <ServerCore/Service.h>
#include "Protocol/Enum.pb.h"


using namespace Craft;

Player::Player(const Vector2& position, uint64 objectId)
	: super(L"◑", position, Craft::Color::Green, objectId)
{
	sortingOrder = 10;
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

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

		_direction = Protocol::DirectionType::DIR_RIGHT;
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x -= 1;

		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_LEFT);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

		_direction = Protocol::DirectionType::DIR_LEFT;
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y -= 1;

		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_UP);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

		_direction = Protocol::DirectionType::DIR_UP;
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y += 1;

		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_DOWN);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

		_direction = Protocol::DirectionType::DIR_DOWN;
	}

	int64 b = _nextSyncTick - _prevSyncTick;

	float delta = (b > 0) ? (_nextSyncTick - GDelayedTickCount) / b : deltaTime;

	_xPos = _prevPos.x + (_nextPos.x - _prevPos.x) * delta;
	_yPos = _prevPos.y + (_nextPos.y - _prevPos.y) * delta; 
	super::SetPosition(Vector2(static_cast<int>(_xPos), static_cast<int>(_yPos)));


	//ASSERT_CRASH(_direction != Protocol::DirectionType::DIR_NONE);
	//int32 direction = static_cast<int32>(_direction);

	//// y축 = 1, x축 = 0
	//int32 axisType = direction / 3;
	//int32 valueType = (direction % 2 == 0) ? 1 : -1;

	//if (axisType == 0)
	//{
	//	_xPos = _xPos + (static_cast<float>(valueType) * _moveSpeed * deltaTime);
	//}
	//else
	//{
	//	_yPos = _yPos + (static_cast<float>(valueType) * _moveSpeed * deltaTime);
	//}

	//super::SetPosition(Vector2(static_cast<int>(_xPos), static_cast<int>(_yPos)));
}

void Player::SetPosition(const Craft::Vector2& newPosition)
{
	Craft::Vector2 pos = Craft::Vector2(newPosition.x / 100, newPosition.y / 100);
	Actor::SetPosition(pos);
	_xPos = static_cast<float>(newPosition.x) / 100;
	_yPos = static_cast<float>(newPosition.y) / 100;
}
