#include "pch.h"
#include "Box.h"

using namespace Craft;

Box::Box(const Vector2& position)
	:super(L"B", position, Color::Red)
{
	// 박스는 이동이 가능하기 때문에 땅과 겹칠 수 있음
	// 땅 보다 높은 우선순위로 설정
	// 박스는 타겟(목표 지점)위에 배치 되어야 하므로 타겟보다 우선 순위가 높아야함
	sortingOrder = 5;

}
