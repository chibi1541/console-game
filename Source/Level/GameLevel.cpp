#include "pch.h"
#include "GameLevel.h"
#include "Actor/Box.h"
#include "Actor/Wall.h"
#include "Actor/Player.h"
#include "Actor/Target.h"
#include "Actor/Ground.h"

using namespace Craft;

bool GameLevel::CanMove(const Vector2& playerPos, const Vector2& nextPos)
{
	// 게임이 클리어 된 상태인 경우 이동 처리 안함.
	if (isGameClear)
		return false;

	// 이동하려는 위치에 어떤 액터가 있는지를 확인할 때 타입을 활용.
	// 박스가 있을 때 로직이 더 복잡하기 때문에 이 처리를 위한 배열
	// 이건 그냥 level 처음 만들때 가지고 있으면 좋지 않은가?
	std::vector<std::shared_ptr<Actor>> boxList;

	for (const std::shared_ptr<Actor>& actor : actorList)
	{
		// 커스텀 RTTI를 사용해서 박스 타입인지 확인
		if (actor->IsTypeOf<Box>())
		{
			boxList.emplace_back(actor);
			continue;
		}
	}

	// 이동하려는 위치에 박스가 있는지 검증
	std::shared_ptr<Actor> boxActor = nullptr;
	for (const std::shared_ptr<Actor> box : boxList)
	{
		if (box->GetPosition() == nextPos)
		{
			boxActor = box;
			break;
		}
	}

	if (boxActor)
	{
		// 박스가 밀 수 있는 위치인지
		// 여긴 다 노말 값므로 거리는 안 곱해도 됨
		Vector2 deltaVec = nextPos - playerPos;

		// 옵션 지식 - 하지만 그래픽스를 요구하는 곳에서는 필수.
		// 위치(좌표) + 벡터(크기, 방향을 가지는 데이터).
		// 벡터의 정의에 따르면 위치 정보는 없음.
		// 하지만 이 덧셈은 수학적으로 정의되지 않음.
		// 원래는 안되는데 이게 가능하도록 별도로 정의함.
		// -> 동차좌표계(위치, 벡터) | 아핀 변환.
		// (x, y, w): w가 0이면 벡터 | w가 1이면 위치.
		// (x, y, z, w): w가 0이면 벡터 | w가 1이면 위치.
		// (x1, y1, 1) + (x2, y2, 0) = (x1+x2, y1+y2, 1)
		// (x1, y1, 1) + (x2, y2, 1) = (x1+x2, y1+y2, 2)
		Vector2 newBoxPos = boxActor->GetPosition() + deltaVec;

		for (const std::shared_ptr<Actor>& actor : actorList)
		{
			// 벽이랑 박스가 이동 방향에 있는 경우 박스 이동이 안됨
			if (actor->IsTypeOf<Wall>() || actor->IsTypeOf<Box>())
			{
				// 이 경우 벽 때문에 박스가 안밀림
				if (actor->GetPosition() == newBoxPos)
					return false;
			}
		}

		// 벽이나 박스가 없다면 박스도 밀고 플레이어도 밀 수 있게 함
		boxActor->SetPosition(newBoxPos);
		return true;
	}

	// 바로 벽이 있는 경우
	for (const std::shared_ptr<Actor>& actor : actorList)
	{
		if (actor->IsTypeOf<Wall>())
		{
			if (actor->GetPosition() == nextPos)
				return false;
		}
	}

	return true;
}

void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	// 파일을 읽어서 맵 로드
	LoadMap("Map.txt");
}

void GameLevel::Draw()
{
	Level::Draw();
}

void GameLevel::LoadMap(const std::string& filename)
{
	// 최종 경로 조립
	std::string path = std::string("../Assets/") + filename;

	// 파일 열기
	FILE* file = nullptr;
	fopen_s(&file, path.c_str(), "rt");
	ASSERT_CRASH(file);

	// 파일 내용을 저장할 버퍼(데이터 저장공간) 확인
	// 파일 길이 확인
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);

	// 파일 제일 끝위치를 구한 다음에 다시 처음으로 되돌리기
	rewind(file);

	char* buffer = new char[fileSize];

	// 파일 읽기
	size_t readSize = fread(buffer, sizeof(char), fileSize, file);

	ASSERT_CRASH(readSize > 0);

	// TODO : 읽은 데이터를 기반으로 로직 제작
	// 1. 화면에 액터를 그리기

	// 문자열에 저장된 값을 접근할 때 사용할 인덱스
	int index = 0;
	Vector2 position = Vector2::Zero;

	// 액터 생성에 사용할 위치 값.
	while (true)
	{
		// 루프 종료 조건
		// 내용을 모두 읽었는지
		if (index >= fileSize)
		{
			break;
		}

		char mapCharacter = buffer[index];

		// 인덱스 증가 처리.
		++index;

		if (mapCharacter == '\n')
		{
			++position.y;
			position.x = 0;
			continue;
		}

		switch (mapCharacter)
		{
			// 벽
		case '#':
			SpawnActor<Wall>(position);
			break;

			// 땅
		case '.':
			SpawnActor<Ground>(position);
			break;

			// 플레이어
		case 'p':
			SpawnActor<Ground>(position);
			SpawnActor<Player>(position);
			break;

			// 박스
		case 'b':
			SpawnActor<Ground>(position);
			SpawnActor<Box>(position);
			break;

			// 타겟
		case 't':
			SpawnActor<Target>(position);

			++targetScore;

			break;
		}

		++position.x;
	}

	// 사용한 버퍼 해제
	delete[] buffer;
	buffer = nullptr;

	// 파일 닫기
	fclose(file);
	file = nullptr;
}
