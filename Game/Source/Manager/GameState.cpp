#include "pch.h"
#include "GameState.h"

void GameState::UpdatePlayerInfo(const Client::PlayerInfo& player)
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
