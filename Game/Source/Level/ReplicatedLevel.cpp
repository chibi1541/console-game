#include "pch.h"
#include "ReplicatedLevel.h"
#include "Actor/ReplicatedActor.h"
#include "Globals.h"
#include "Render/Renderer.h"
#include <string>
#include "Actor/Player.h"
#include "Actor/LocalPlayer.h"
#include "Actor/RemotePlayer.h"
#include "Actor/Wall.h"
#include "Actor/Item.h"
#include "Utils/ObjectIdHandler.h"
#include "Actor/OtherPlayer.h"

using namespace Protocol;

void ReplicatedLevel::OnInitialized()
{
	super::OnInitialized();
	_gameState = make_unique<GameState>();
	std::vector<wstring> one = {
		L"  █  ",
		L" ██  ",
		L"  █  ",
		L"  █  ",
		L"█████"
	};
	numbers.emplace_back(one);

	std::vector<wstring> two = {
		L"█████",
		L"    █",
		L"█████",
		L"█    ",
		L"█████"
	};
	numbers.emplace_back(two);

	std::vector<wstring> three = {
		L"█████",
		L"    █",
		L" ████",
		L"    █",
		L"█████"
	};
	numbers.emplace_back(three);

	std::vector<wstring> four = {
		L"█   █",
		L"█   █",
		L"█████",
		L"    █",
		L"    █"
	};
	numbers.emplace_back(four);

	std::vector<wstring> five = {
		L"█████",
		L"█    ",
		L"████ ",
		L"    █",
		L"████ "
	};
	numbers.emplace_back(five);


	std::vector<wstring> w = {
		L"█   █",
		L"█   █",
		L"█ █ █",
		L"██ ██",
		L"█   █"
	};
	win.emplace_back(w);
	std::vector<wstring> i = {
		L"  █  ",
		L"  █  ",
		L"  █  ",
		L"  █  ",
		L"  █  "
	};
	win.emplace_back(i);
	std::vector<wstring> n = {
		L"█   █",
		L"██  █",
		L"█ █ █",
		L"█  ██",
		L"█   █"
	};
	win.emplace_back(n);

	std::vector<wstring> l = {
		L"█    ",
		L"█    ",
		L"█    ",
		L"█    ",
		L"█████"
	};
	lose.emplace_back(l);

	std::vector<wstring> o = {
		L" ███ ",
		L"█   █",
		L"█   █",
		L"█   █",
		L" ███ "
	};
	lose.emplace_back(o);

	std::vector<wstring> s = {
		L"█████",
		L"█    ",
		L"█████",
		L"    █",
		L"█████"
	};
	lose.emplace_back(s);

	std::vector<wstring> e = {
		L"█████",
		L"█    ",
		L"████ ",
		L"█    ",
		L"█████"
	};
	lose.emplace_back(e);

	SetRandomSeed32();
	SetRandomSeed64();
}

void ReplicatedLevel::Push(JobRef job)
{
	WRITE_LOCK;
	_jobQueue.push(job);
}

void ReplicatedLevel::InitLocalPlayer(Client::PlayerInfo localPlayer)
{
	ASSERT_CRASH(_gameState);
	_gameState->InitLocalPlayer(localPlayer);
}

void ReplicatedLevel::UpdateReplicated()
{
	WRITE_LOCK;
	while (false == _jobQueue.empty())
	{
		JobRef job = _jobQueue.front();
		job->Execute();
		_jobQueue.pop();
	}
}

void ReplicatedLevel::AddLevelSnapshot(uint64 syncTick, Protocol::S_UPDATE_ROOM pkt)
{
	_syncQueue.push(LevelSyncData(syncTick, pkt));
}

void ReplicatedLevel::UpdateSyncData(LevelSyncData prevSyncData, LevelSyncData nextSyncData)
{
	vector<PlayerRef> players = FindActors<Player>();
	for (PlayerRef player : players)
	{
		for (auto playerInfo : prevSyncData.pkt.players())
		{
			if (player->GetObjectId() == playerInfo.head().actor().objectid())
			{
				Client::PlayerInfo info;
				info.SetPlayerInfo(playerInfo);
				_gameState->UpdatePlayerInfo(info);

				// 게임 오버인 경우 처리
				if (info.bGameOver == true)
				{
					player->DestroyPlayer();
					break;
				}

				const Protocol::HeadData& head = playerInfo.head();
				ActorInfo actorInfo = head.actor();
				player->SetPrevSyncTick(prevSyncData.syncTick);
				player->SetPrevSyncPos(Craft::Vector2(actorInfo.pos().x(), actorInfo.pos().y()));
				player->SetSyncDirection(head.dir());

				if (head.trails_size() > 0)
				{
					player->UpdateTrailInfo(head.trails());
				}

				break;
			}
		}

		for (auto playerInfo : nextSyncData.pkt.players())
		{
			// 파괴 예정이므로 갱신하지 않음
			if (player->IsActive() == false)
				continue;

			if (player->GetObjectId() == playerInfo.head().actor().objectid())
			{
				const Protocol::HeadData& head = playerInfo.head();
				const ActorInfo& actorInfo = head.actor();
				player->SetNextSyncTick(nextSyncData.syncTick);
				player->SetNextSyncPos(Craft::Vector2(actorInfo.pos().x(), actorInfo.pos().y()));

				if (head.trails_size() > 0)
				{
					player->UpdateNextTrailInfo(head.trails());
				}

				break;
			}
		}
	}


	vector<Craft::Vector2> alreadyHas;
	vector<ReplActorRef> actors = FindActors<ReplicatedActor>();
	for (ReplActorRef actor : actors)
	{
		if (ObjectIdHandler::GetObjectType(actor->GetObjectId()) == ObjectType::OBJECT_SNAKE_HEAD)
			continue;

		bool bDestory = true;

		for (auto field : prevSyncData.pkt.fielddata())
		{
			Craft::Vector2 pos = Craft::Vector2(field.pos().x(), field.pos().y());
			if (pos == actor->GetPosition())
			{
				alreadyHas.push_back(pos);
				bDestory = false;
				break;
			}
		}

		if (bDestory)
			actor->Destroy();
	}

	for (auto field : prevSyncData.pkt.fielddata())
	{
		bool bSpawn = true;
		Craft::Vector2 npos = Craft::Vector2(field.pos().x(), field.pos().y());
		for (auto pos : alreadyHas)
		{
			if (pos == npos)
			{
				bSpawn = false;
				break;
			}
		}

		if (bSpawn)
			SpawnActor<Item>(npos, 0);
	}
}

void ReplicatedLevel::DestroyReplicatedActor(uint64 objectId)
{
	vector<ReplActorRef> actors = FindActors<ReplicatedActor>();
	for (ReplActorRef actor : actors)
	{
		if (actor->GetObjectId() == objectId)
		{
			// temp 지워야 할지도?
			// 파괴 예약
			actor->Destroy();
			break;
		}
	}
}

void ReplicatedLevel::GameStart(float remainCount)
{
	ASSERT_CRASH(_gameState);

	_gameState->bGameStart = true;
	_gameState->remainCount = remainCount;
}

void ReplicatedLevel::CountGameStart(float deltaTime)
{
	_gameState->remainCount -= deltaTime;

	if (_gameState->remainCount <= 0.f)
	{
		GIsGameStart = true;
		_gameState->remainCount = 0.f;
		return;
	}
	else
	{
		int32 index = static_cast<int32>(_gameState->remainCount);
		index = min(index, numbers.size() - 1);
		int32 count = 0;
		for (auto text : numbers[index])
		{
			Craft::Renderer::Get().Submit(text, Craft::Vector2((_width / 2) - 3, (_height / 2) - 3 + count), Craft::Color::BrightWhite, 50);
			++count;
		}
	}

	vector<Client::PlayerInfo> players = _gameState->GetAllPlayerInfo();
	std::sort(players.begin(), players.end(), std::greater());
	int32 indexCount = 0;
	for (Client::PlayerInfo player : players)
	{
		wstring wName = FileUtils::Convert(player.name);
		wstring wMark = (player.bGameOver) ? L"☠ " : L"§";
		if (player.userId == GLocalUserId)
		{
			Craft::Renderer::Get().Submit(std::format(L"{} {}(You) ", wMark, wName), Craft::Vector2(85, (indexCount * 3) + 1), player.color, 50);
		}
		else
		{
			Craft::Renderer::Get().Submit(std::format(L"{} {} ", wMark, wName), Craft::Vector2(85, (indexCount * 3) + 1), player.color, 50);
		}

		Craft::Renderer::Get().Submit(std::format(L" | Score : {} ", player.score), Craft::Vector2(85, (indexCount * 3) + 2), Craft::Color::BrightWhite, 50);

		++indexCount;
	}

}

void ReplicatedLevel::WaitGameStart(float deltaTime)
{
	static float tickCount = 0.f;
	tickCount += deltaTime * 5;

	const WCHAR spinner[] = { L'|', L'/', L'─', L'\\' };
	WCHAR cur = spinner[static_cast<int32>(tickCount) % 4];
	Craft::Renderer::Get().Submit(std::format(L"{} 플레이어를 기다리는 중...({}/{}) ", std::wstring(1, cur), _gameState->GetCurPlayerCount(), GNeedPlayerCount), Craft::Vector2(85, _height - 5), Craft::Color::BrightWhite, 50);
}

void ReplicatedLevel::ShowGameResult()
{
	if(GWin)
	{
		int32 offset = 0;
		for(auto strings : win)
		{
			int32 index = 0;
			for (auto string : strings)
			{
				Craft::Renderer::Get().Submit(string, Craft::Vector2(85 + (offset * 6), _height - 8 + index), Craft::Color::BrightWhite, 50);
				++index;
			}

			++offset;
		}
	}
	else
	{
		int32 offset = 0;

		for (auto strings : lose)
		{
			int32 index = 0;
			for (auto string : strings)
			{
				Craft::Renderer::Get().Submit(string, Craft::Vector2(85 + (offset * 6), _height - 8 + index), Craft::Color::BrightWhite, 50);
				++index;
			}
			++offset;
		}
	}
}

void ReplicatedLevel::InitField(uint32 width, uint32 height)
{
	_width = width;
	_height = height;

	// 벽 생성
	for (uint32 idx = 0; idx < width; ++idx)
	{
		//if(idx % 3 == 0)
		SpawnActor<Wall>(Craft::Vector2(idx, 0));
		//else if(idx % 3 == 2)
		SpawnActor<Wall>(Craft::Vector2(idx, height - 1));
	}

	for (uint32 idx = 0; idx < height; ++idx)
	{
		//if (idx % 3 == 0)
		SpawnActor<Wall>(Craft::Vector2(0, idx));
		//else if(idx % 3 == 2)
		SpawnActor<Wall>(Craft::Vector2(width - 1, idx));
	}
}

void ReplicatedLevel::InitPlayers(vector<Protocol::PlayerInfo> players)
{
	ASSERT_CRASH(_gameState);

	for (const Protocol::PlayerInfo& player : players)
	{
		Client::PlayerInfo info;
		info.SetPlayerInfo(player);

		if (info.userId == GLocalUserId)
		{
			_gameState->UpdatePlayerInfo(info);
			const Protocol::HeadData& head = player.head();

			Craft::Vector2 spawnPos = Craft::Vector2(head.actor().pos().x() / 100, head.actor().pos().y() / 100);
			PlayerRef player = SpawnActor<LocalPlayer>(spawnPos, info.color, head.actor().objectid());
			player->SetMoveSpeed(head.movespeed());
			player->SetSyncDirection(head.dir());
		}
		else
		{
			_gameState->AddPlayerInfo(info);
			const Protocol::HeadData& head = player.head();

			Craft::Vector2 spawnPos = Craft::Vector2(head.actor().pos().x() / 100, head.actor().pos().y() / 100);
			PlayerRef player = SpawnActor<RemotePlayer>(spawnPos, info.color, head.actor().objectid());
			player->SetMoveSpeed(head.movespeed());
			player->SetSyncDirection(head.dir());
		}
	}
}

void ReplicatedLevel::SpawnPlayer(const Protocol::PlayerInfo& player)
{
	ASSERT_CRASH(_gameState);

	if (player.id() == GLocalUserId)
		return;

	Client::PlayerInfo info;
	info.SetPlayerInfo(player);
	_gameState->AddPlayerInfo(info);
	const Protocol::HeadData& head = player.head();

	Craft::Vector2 spawnPos = Craft::Vector2(head.actor().pos().x() / 100, head.actor().pos().y() / 100);
	PlayerRef actor = SpawnActor<RemotePlayer>(spawnPos, info.color, head.actor().objectid());
	actor->SetMoveSpeed(head.movespeed());
	actor->SetSyncDirection(head.dir());
}

void ReplicatedLevel::Tick(float deltaTime)
{
	UpdateReplicated();

	if (_gameState->bGameStart == true && _gameState->remainCount > 0.f)
	{
		CountGameStart(deltaTime);
	}
	else if (_gameState->bGameStart == false)
	{
		WaitGameStart(deltaTime);
	}

	if (GIsGameStart)
	{

		// POSTPHONE_TICK 값만큼 이전 서버 데이터로 갱신 및 보간
		GDelayedTickCount = ::GetTickCount64() - POSTPHONE_TICK;

		// Server SyncData 변경
		if (GDelayedTickCount >= _targetTickCount)
		{
			LevelSyncData prevData;
			LevelSyncData nextData;

			do
			{
				if (false == _syncQueue.empty())
				{
					LevelSyncData front = _syncQueue.front();
					// 이전 데이터가 있다면 기준 데이터로 설정하고 queue에서 제거
					if (front.syncTick < GDelayedTickCount)
					{
						prevData = front;
						_syncQueue.pop();
					}
					else
					{
						// 목표 싱크 데이터 갱신
						nextData = front;
						_targetTickCount = front.syncTick;
					}
				}
			} while (false == _syncQueue.empty() && nextData.syncTick == 0);

			UpdateSyncData(prevData, nextData);
		}
	}

	super::Tick(deltaTime);

	// 게임 UI 출력
	vector<Client::PlayerInfo> players = _gameState->GetAllPlayerInfo();
	std::sort(players.begin(), players.end(), std::greater());
	int32 indexCount = 0;
	for (Client::PlayerInfo player : players)
	{
		wstring wName = FileUtils::Convert(player.name);
		wstring wMark = (player.bGameOver) ? L"☠ " : L"§";
		if (player.userId == GLocalUserId)
		{
			Craft::Renderer::Get().Submit(std::format(L"{} {}(You) ", wMark, wName), Craft::Vector2(85, (indexCount * 3) + 1), player.color, 50);
		}
		else
		{
			Craft::Renderer::Get().Submit(std::format(L"{} {} ", wMark, wName), Craft::Vector2(85, (indexCount * 3) + 1), player.color, 50);
		}

		Craft::Renderer::Get().Submit(std::format(L" | Score : {} ", player.score), Craft::Vector2(85, (indexCount * 3) + 2), Craft::Color::BrightWhite, 50);

		++indexCount;
	}

	if(GGameOver)
	{
		ShowGameResult();
	}
}
