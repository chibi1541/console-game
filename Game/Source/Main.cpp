#pragma once
#include "pch.h"
#include "Engine/Engine.h"
#include "Level/TestLevel.h"
#include "Protocol/ServerPacketHandler.h"
#include "ServerCore/ThreadManager.h"
#include "ServerCore/Service.h"


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

int main()
{
	ServerPacketHandler::Init();

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		1
	);

	ASSERT_CRASH(service->Start());

	Craft::Engine engine;
	Engine::Get().AddNewLevel<TestLevel>();

	// 패킷만 처리하는 쓰레드를 추가
	GThreadManager->Launch([=]()
		{
			while (true)
			{
				service->GetIocpCore()->Dispatch();
			}
		});

	Engine::Get().Run();

	GThreadManager->Join();
}