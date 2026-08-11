#include "pch.h"
#include "ReplicatedLevel.h"
#include "Actor/ReplicatedActor.h"

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

void ReplicatedLevel::UpateLevelReplicated(Protocol::S_UPDATE_ROOM pkt)
{
	vector<std::shared_ptr<ReplicatedActor>> actors = FindActors<ReplicatedActor>();

	for (std::shared_ptr<ReplicatedActor> actor : actors)
	{
		for (auto actorInfo : pkt.actors())
		{
			if (actor->GetObjectId() == actorInfo.objectid())
			{
				actor->SetPosition(Craft::Vector2(actorInfo.pos().x(), actorInfo.pos().y()));
			}
		}
	}
}

void ReplicatedLevel::Tick(float deltaTime)
{
	UpdateReplicated();

	super::Tick(deltaTime);
}
