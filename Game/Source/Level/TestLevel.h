#pragma once
#include <memory>
#include "Level/Level.h"

using namespace Craft;

class TestLevel : public Craft::Level
{
public:
	virtual void OnInitialized() override;
private:
	virtual void Tick(float deltaTime) override;
};

