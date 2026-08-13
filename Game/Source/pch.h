#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#ifdef _DEBUG
#pragma comment(lib, "CraftEngine\\Debug\\CraftEngine.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#else
#pragma comment(lib, "CraftEngine\\Release\\CraftEngine.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobufd.lib")
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#endif

#include "Utils/CorePch.h"
#include "ServerCore/CorePch.h"

using ServerSessionRef = shared_ptr<class ServerSession>;
using ReplActorRef = shared_ptr<class ReplicatedActor>;
using ReplLevelRef = shared_ptr<class ReplicatedLevel>;
