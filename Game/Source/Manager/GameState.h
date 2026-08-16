#pragma once
#include "Protocol/Struct.pb.h"

NAMESPACE_BEGIN(Client)

struct PlayerInfo
{
	uint64 userId = 0;
	std::string name = {};
	uint32 score = 0;
	uint64 objectId = 0;
	bool bGameOver = false;

	void SetPlayerInfo(const PlayerInfo& other )
	{
		userId = other.objectId;
		name = other.name;
		score = other.score;
		objectId = other.objectId;
		bGameOver = other.bGameOver;
	}

	void SetPlayerInfo(const Protocol::PlayerInfo& other)
	{
		userId = other.id();
		name = other.name();
		score = other.score();
		bGameOver = other.isgameover();
		objectId = static_cast<uint64>(other.head().actor().objectid());
	}
};

NAMESPACE_END

class GameState
{
public:
	GameState() = default;
	~GameState() = default;

	void InitLocalPlayer(const Client::PlayerInfo& player);
	void AddPlayerInfo(const Client::PlayerInfo& player);
	void UpdatePlayerInfo(const Client::PlayerInfo& player);

	const Client::PlayerInfo& GetLocalPlayer() const;
	const vector<Client::PlayerInfo>& GetRemotePlayerInfo() const { return _playerInfos; }

private:
	vector<Client::PlayerInfo> _playerInfos;
};

extern uint64 GLocalUserId;
extern uint64 GLocalActorId;



