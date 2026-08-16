#pragma once
#include "Protocol/Struct.pb.h"
#include "Utils/HelperFuncs.h"

NAMESPACE_BEGIN(Client)

struct PlayerInfo
{
	uint64 userId = 0;
	std::string name = {};
	Craft::Color color = Craft::Color::Green;
	uint32 score = 0;
	uint64 objectId = 0;
	bool bGameOver = false;

	inline bool operator<(const PlayerInfo& other)
	{
		return score < other.score;
	}

	inline bool operator>(const PlayerInfo& other)
	{
		return score > other.score;
	}

	void SetPlayerInfo(const PlayerInfo& other )
	{
		userId = other.objectId;
		name = other.name;
		score = other.score;
		color = other.color;
		objectId = other.objectId;
		bGameOver = other.bGameOver;
	}

	void SetPlayerInfo(const Protocol::PlayerInfo& other)
	{
		userId = other.id();
		name = other.name();
		score = other.score();
		color = Utils::ConvertColor(other.color());
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
	vector<Client::PlayerInfo> GetAllPlayerInfo() const { return _playerInfos; }

	uint32 GetCurPlayerCount() const {return static_cast<uint32>(_playerInfos.size()); }

	bool bGameStart = false;
	float remainCount = 0.f;

private:
	vector<Client::PlayerInfo> _playerInfos;


};

extern uint64 GLocalUserId;
extern uint64 GLocalActorId;
extern uint32 GNeedPlayerCount;
extern bool GIsGameStart;
extern bool GGameOver;
extern bool GWin;


