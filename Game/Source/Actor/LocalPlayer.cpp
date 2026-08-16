#include "pch.h"
#include "LocalPlayer.h"
#include "Input/Input.h"
#include "Globals.h"
#include "Protocol/ServerPacketHandler.h"
#include "Protocol/Protocol.pb.h"
#include "ServerCore/Service.h"
#include "Protocol/Enum.pb.h"
#include "Manager/GameState.h"

using namespace Craft;

LocalPlayer::LocalPlayer(const Craft::Vector2& position, uint64 objectId)
	: super(position, Craft::Color::Green, objectId)
{
	
}

LocalPlayer::LocalPlayer(const Craft::Vector2& position, Craft::Color color, uint64 objectId)
	: super(position, color, objectId)

{
}

void LocalPlayer::Tick(float deltaTime)
{
	
	// TODO : AI용 로직을 추가
	// 종료처리
if(GIsAI == false)
	ProcessPlayerInput();
else
	ProcessAI(deltaTime);

	super::Tick(deltaTime);
}

void LocalPlayer::ProcessPlayerInput()
{
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Protocol::C_EXIT_GAME pkt;
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);

		// 엔진 종료처리
		QuitGame();
		GEngineQuit = true;
		return;
	}

	if (GIsGameStart == false || GGameOver == true)
		return;

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		// TODO : 클라에서 서버 쪽으로 패킷 보내는 로직도 별도의 분리가 필요
		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_RIGHT);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_LEFT);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_UP);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Protocol::C_MOVE_ACTOR pkt;
		pkt.set_newdir(Protocol::DirectionType::DIR_DOWN);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GService.get()->Broadcast(sendBuffer);
	}
}

void LocalPlayer::ProcessAI(float deltaTime)
{
	_elapsedTime -= deltaTime;
	AxisType axisType = static_cast<AxisType>(static_cast<int32>(syncDir) / static_cast<int32>(AxisType::NUMBER));
	int32 sign = static_cast<int32>(syncDir % 2);
	sign = (sign == 1) ? sign = -1 : sign = 1;
	bool bWarning = false;

	if(axisType == AxisType::X)
	{
		int32 front = position.x + sign * WARNING_VALUE;
		if(front <= 0 || front >= 80)
		{
			bWarning = true;
		}
	}
	else
	{
		int32 front = position.y + sign * WARNING_VALUE;
		if (front <= 0 || front >= 30)
		{
			bWarning = true;
		}
	}

	if( bWarning ||_elapsedTime <= 0.f )
	{
		float delta = FRandomRange(-1.0f, 1.0f);

		_elapsedTime = 2.f + delta;
		
		if (axisType == AxisType::X)
		{
			Protocol::C_MOVE_ACTOR pkt;

			if (position.y >= (80 - WARNING_VALUE))
			{
				pkt.set_newdir(Protocol::DirectionType::DIR_UP);
			}
			else if(position.y <= WARNING_VALUE)
			{
				pkt.set_newdir(Protocol::DirectionType::DIR_DOWN);
			}
			else
			{
				pkt.set_newdir((delta > 0.f) ? Protocol::DirectionType::DIR_UP : Protocol::DirectionType::DIR_DOWN);

			}

			SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			GService.get()->Broadcast(sendBuffer);
		}
		else
		{
			Protocol::C_MOVE_ACTOR pkt;

			if (position.x >= (80 - WARNING_VALUE) )
			{
				pkt.set_newdir(Protocol::DirectionType::DIR_LEFT);
			}
			else if(position.x <= WARNING_VALUE)
			{
				pkt.set_newdir(Protocol::DirectionType::DIR_RIGHT);
			}
			else
			{
				pkt.set_newdir((delta > 0.f) ? Protocol::DirectionType::DIR_LEFT : Protocol::DirectionType::DIR_RIGHT);
			}

			SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			GService.get()->Broadcast(sendBuffer);
		}
	}
}
