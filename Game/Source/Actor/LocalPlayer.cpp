#include "pch.h"
#include "LocalPlayer.h"
#include "Input/Input.h"
#include "Globals.h"
#include "Protocol/ServerPacketHandler.h"
#include "Protocol/Protocol.pb.h"
#include "ServerCore/Service.h"
#include "Protocol/Enum.pb.h"

using namespace Craft;

LocalPlayer::LocalPlayer(const Craft::Vector2& position, uint64 objectId)
	: super(position, Craft::Color::Green, objectId)
{
	
}

void LocalPlayer::Tick(float deltaTime)
{
	
	// TODO : AI용 로직을 추가
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

	super::Tick(deltaTime);
}
