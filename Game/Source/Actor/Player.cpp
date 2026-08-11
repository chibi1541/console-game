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

	// 종료처리
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
		return;
	}

	// 지금 상태면 대각선 이동돼서 박스 밀리는 판단이 애매해지지 않나?
	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x += 1;

		// TODO : 클라에서 서버 쪽으로 패킷 보내는 로직도 별도의 분리가 필요
		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_RIGHT);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x -= 1;

		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_LEFT);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y -= 1;

		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_UP);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y += 1;

		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_DOWN);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

	}
}
