#include "pch.h"
#include "ReplicatedLevel.h"
#include "Actor/ReplicatedActor.h"
#include "Globals.h"
#include "Render/Renderer.h"
#include <string>

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

	//vector<std::shared_ptr<ReplicatedActor>> actors = FindActors<ReplicatedActor>();

	//for (std::shared_ptr<ReplicatedActor> actor : actors)
	//{
	//	for (auto actorInfo : pkt.actors())
	//	{
	//		if (actor->GetObjectId() == actorInfo.objectid())
	//		{
	//			actor->SetLastSyncTick(syncTick);
	//			actor->SetLastSyncPos(Craft::Vector2(actorInfo.pos().x(), actorInfo.pos().y()));
	//			break;
	//		}
	//	}
	//}
}

void ReplicatedLevel::UpdateSyncData(LevelSyncData prevSyncData, LevelSyncData nextSyncData)
{
	vector<ReplActorRef> actors = FindActors<ReplicatedActor>();

	for (ReplActorRef actor : actors)
	{
		for (auto actorInfo : prevSyncData.pkt.actors())
		{
			if (actor->GetObjectId() == actorInfo.objectid())
			{
				actor->SetPrevSyncTick(prevSyncData.syncTick);
				actor->SetPrevSyncPos(Craft::Vector2(actorInfo.pos().x() / 100, actorInfo.pos().y() / 100));
				break;
			}
		}

		for (auto actorInfo : nextSyncData.pkt.actors())
		{
			if (actor->GetObjectId() == actorInfo.objectid())
			{
				actor->SetNextSyncTick(nextSyncData.syncTick);
				actor->SetNextSyncPos(Craft::Vector2(actorInfo.pos().x() / 100, actorInfo.pos().y() / 100));
				break;
			}
		}
	}
}

void ReplicatedLevel::DestroyReplicatedActor(uint64 objectId)
{
	vector<ReplActorRef> actors = FindActors<ReplicatedActor>();
	for (ReplActorRef actor : actors)
	{
		if (actor->GetObjectId() == objectId)
		{
			// 파괴 예약
			actor->Destroy();
			break;
		}
	}
}

void ReplicatedLevel::Tick(float deltaTime)
{
	const std::wstring frameRate = std::format(L"FPS : {}", 1 / deltaTime);

	Craft::Renderer::Get().Submit(frameRate, Craft::Vector2::Zero, Craft::Color::BrightWhite, 100);

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
