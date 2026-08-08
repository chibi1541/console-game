#pragma once

#include "Level/Level.h"

// 倉庫番(そうこばん) 창고지기
// 게임 레벨 클래스
class GameLevel : public Craft::Level
{
	TYPE_DECLARATIONS(GameLevel, Level)

public:
	// 플레이어가 이동하려는 위치가 이동 가능하지를 판단해주는 함수.
	bool CanMove(const Craft::Vector2& playerPos, const Craft::Vector2& nextPos);

private:
	// 레벨 초기화 함수
	virtual void OnInitialized() override;

	// Draw 이벤트 함수
	virtual void Draw() override;

	// 맵 로드 함수
	void LoadMap(const std::string& filename);

private:
	int targetScore = 0;

	bool isGameClear = false;
};

