#include "pch.h"
#include "ReplicatedLevel.h"
#include "Actor/ReplicatedActor.h"
#include "Globals.h"
#include "Render/Renderer.h"
#include <string>
#include "Actor/Player.h"
#include "Actor/Wall.h"
#include "Actor/Item.h"
#include "Utils/ObjectIdHandler.h"

using namespace Protocol;

void ReplicatedLevel::OnInitialized()
{
	super::OnInitialized();
}

void ReplicatedLevel::Push(JobRef job)
{
	WRITE_LOCK;
	_jobQueue.push(job);
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
		for (auto headInfo : prevSyncData.pkt.heads())
		{
			if (player->GetObjectId() == headInfo.actor().objectid())
			{
				ActorInfo actorInfo = headInfo.actor();
				player->SetPrevSyncTick(prevSyncData.syncTick);
				player->SetPrevSyncPos(Craft::Vector2(actorInfo.pos().x(), actorInfo.pos().y()));
				player->SetSyncDirection(headInfo.dir());

				if (headInfo.trails_size() > 0)
				{
					player->UpdateTrailInfo(headInfo.trails());
				}

				break;
			}
		}

		for (auto headInfo : nextSyncData.pkt.heads())
		{
			if (player->GetObjectId() == headInfo.actor().objectid())
			{
				ActorInfo actorInfo = headInfo.actor();
				player->SetNextSyncTick(nextSyncData.syncTick);
				player->SetNextSyncPos(Craft::Vector2(actorInfo.pos().x(), actorInfo.pos().y()));

				if (headInfo.trails_size() > 0)
				{
					player->UpdateNextTrailInfo(headInfo.trails());
				}

				break;
			}
		}
	}


	vector<Craft::Vector2> alreadyHas;
	vector<ReplActorRef> actors = FindActors<ReplicatedActor>();
	for (ReplActorRef actor : actors)
	{
		if(ObjectIdHandler::GetObjectType(actor->GetObjectId()) == ObjectType::OBJECT_SNAKE_HEAD)
			continue;

		bool bDestory = true;

		for (auto field : prevSyncData.pkt.fielddata())
		{
			Craft::Vector2 pos = Craft::Vector2(field.pos().x(), field.pos().y());
			if(pos == actor->GetPosition())
			{
				alreadyHas.push_back(pos);
				bDestory = false;
				break;	
			}
		}

		if(bDestory)
			actor->Destroy();
	}

	for (auto field : prevSyncData.pkt.fielddata())
	{
		bool bSpawn = true;
		Craft::Vector2 npos = Craft::Vector2(field.pos().x(), field.pos().y());
		for(auto pos : alreadyHas)
		{
			if(pos == npos)
			{
				bSpawn = false;
				break;
			}
		}

		if(bSpawn)
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

void ReplicatedLevel::InitField(uint32 width, uint32 height)
{
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

void ReplicatedLevel::Tick(float deltaTime)
{
	//const std::wstring frameRate = std::format(L"FPS : {}", 1 / deltaTime);

	//const std::wstring syncQueueSize = std::format(L"Sync Queue Size : {}", _syncQueue.size());

	//Craft::Renderer::Get().Submit(frameRate, Craft::Vector2::Zero, Craft::Color::BrightWhite, 100);
	//Craft::Renderer::Get().Submit(syncQueueSize, Craft::Vector2(0,1), Craft::Color::BrightWhite, 100);

	UpdateReplicated();

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

	super::Tick(deltaTime);

}
