#pragma once

class Wall : public Craft::Actor
{
	TYPE_DECLARATIONS(Wall, Craft::Actor);

public:
	Wall(const Craft::Vector2& position);

private:
	vector<std::wstring> animations;
};

