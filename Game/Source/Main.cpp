#pragma once
#include "pch.h"
#include "Engine/Engine.h"
#include "Level/ReplicatedLevel.h"
#include "Protocol/ServerPacketHandler.h"
#include "ServerCore/ThreadManager.h"
#include "ServerCore/Service.h"
#include "Globals.h"


class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected()
	{
		Protocol::C_LOGIN pkt;
		
		std::string name = (GIsAI == false) ? "Player" : "Ai";
		pkt.set_name(name);
		
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Send(sendBuffer);
	}

	virtual void OnDisconnected()
	{

	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		PacketSessionRef session = static_pointer_cast<PacketSession>(GetSessionRef());

		ServerPacketHandler::HandlePacket(session, buffer, len);
	}

	virtual void OnSend(int32 len) override
	{
		//cout << "OnSend Len = " << len << endl;
	}
};

int main(int argc, char* argv[])
{
	for(int i = 1; i < argc ; i++)
	{
		std::string arg = argv[i];
		if(arg == "--ai")
			GIsAI = true;
	}

	ServerPacketHandler::Init();

	GService = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		1
	);

	ASSERT_CRASH(GService->Start());

	Craft::Engine engine;
	Craft::Engine::Get().AddNewLevel<ReplicatedLevel>();

	// 패킷만 처리하는 쓰레드를 추가
	GThreadManager->Launch([=]()
		{
			while (true)
			{
				GService->GetIocpCore()->Dispatch(10);

				if(GEngineQuit)
					break;
			}
		});

	Craft::Engine::Get().Run();

	GThreadManager->Join();
}