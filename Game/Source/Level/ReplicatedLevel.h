#pragma once
#include "Level/Level.h"
#include "ServerCore/Job.h"
#include "Protocol/Protocol.pb.h"

class ReplicatedLevel : public Craft::Level
{
	TYPE_DECLARATIONS(ReplicatedLevel, Level)

public:

	// 패킷 처리 관련 
public:
	void Push(JobRef job);


	void UpateLevelReplicated(Protocol::S_UPDATE_ROOM pkt);

protected:
	virtual void UpdateReplicated();

private:
	virtual void Tick(float deltaTime) override;


private:
	// 패킷 처리 관련 작업 큐에서만 사용할 뮤텍스 락
	USE_LOCK;

	queue<JobRef> _jobQueue;

};

