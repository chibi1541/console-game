#include "pch.h"
#include "GameState.h"

uint64 GLocalUserId = 0;
uint64 GLocalActorId = 0;
uint32 GNeedPlayerCount = 0;
bool GIsGameStart = false;
bool GGameOver = false;
bool GWin = false;

void GameState::InitLocalPlayer(const Client::PlayerInfo& player)
{
	GLocalUserId = player.userId;
	GLocalActorId = player.objectId;
	_playerInfos.emplace_back(player);
}

void GameState::AddPlayerInfo(const Client::PlayerInfo& player)
{
	for (auto it = _playerInfos.begin(); it != _playerInfos.end();++it)
	{
		if (player.userId == it->userId)
		{
			it->SetPlayerInfo(player);
			return;
		}
	}

	_playerInfos.emplace_back(player);
}

void GameState::UpdatePlayerInfo(const Client::PlayerInfo& player)
{
	for (Client::PlayerInfo& playerInfo : _playerInfos)
	{
		if (playerInfo.userId == player.userId)
		{
			playerInfo.score = player.score;
			playerInfo.objectId = player.objectId;
			playerInfo.bGameOver = player.bGameOver;
			return;
		}
	}
}

const Client::PlayerInfo& GameState::GetLocalPlayer() const
{
	for(const Client::PlayerInfo& player : _playerInfos)
	{
		if (GLocalUserId == player.userId)
		{
			return player;
		}
	}

	return Client::PlayerInfo();
}
