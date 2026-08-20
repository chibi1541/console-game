#pragma once
#include "Level/Level.h"
#include "ServerCore/Job.h"
#include "Protocol/Protocol.pb.h"
#include "Manager/GameState.h"

struct LevelSyncData
{
	LevelSyncData() = default;

	LevelSyncData(uint64 syncTick, Protocol::S_UPDATE_ROOM pkt)
		: syncTick(syncTick), pkt(pkt)
	{
	}

	uint64 syncTick = 0;
	Protocol::S_UPDATE_ROOM pkt;
};

class ReplicatedLevel : public Craft::Level
{
	enum { POSTPHONE_TICK = 75 };


	TYPE_DECLARATIONS(ReplicatedLevel, Level)

public:
	virtual void OnInitialized() override;


	// 패킷 처리 관련 
public:
	void Push(JobRef job);

	void InitLocalPlayer(Client::PlayerInfo localPlayer);

	void InitField(uint32 width, uint32 height);

	void InitPlayers(vector<Protocol::PlayerInfo> players);

	void SpawnPlayer(const Protocol::PlayerInfo& player);

	void AddLevelSnapshot(uint64 syncTick, Protocol::S_UPDATE_ROOM pkt);

	void UpdateSyncData(LevelSyncData prevSyncData, LevelSyncData nextSyncData);

	void DestroyReplicatedActor(uint64 objectId);

	void GameStart(float remainCount);

	void CountGameStart(float deltaTime);

	void WaitGameStart(float deltaTime);

	void ShowGameResult();

	const vector<Client::PlayerInfo> GetAllPlayerInfo() const;

protected:
	virtual void UpdateReplicated();

private:
	virtual void Tick(float deltaTime) override;


private:
	// 패킷 처리 관련 작업 큐에서만 사용할 뮤텍스 락
	USE_LOCK;

	queue<JobRef> _jobQueue;

	queue<LevelSyncData> _syncQueue;
	uint64 _targetTickCount = 0;

	std::unique_ptr<class GameState> _gameState;
	float _elapsedTime = 0.f;

	std::vector<std::vector<wstring>> numbers;
	std::vector<std::vector<wstring>> win;
	std::vector<std::vector<wstring>> lose;

	uint32 _width;
	uint32 _height;

};
