#pragma once

#include "Protocol/Enum.pb.h"
#include "Math/Color.h"

NAME_SPACE_BEGIN(Utils)

inline static Craft::Color ConvertColor(Protocol::PlayerColor protoColor)
{
	switch(protoColor)
	{
		case Protocol::PlayerColor::COLOR_WHITE:
			return Craft::Color::White;
		case Protocol::PlayerColor::COLOR_GREEN:
			return Craft::Color::Green;
		case Protocol::PlayerColor::COLOR_RED :
			return Craft::Color::Red;
		case Protocol::PlayerColor::COLOR_BLUE:
			return Craft::Color::Blue;
		case Protocol::PlayerColor::COLOR_CYAN:
			return Craft::Color::Cyan;
		case Protocol::PlayerColor::COLOR_PURPLE:
			return Craft::Color::Purple;
		default:
			return Craft::Color::BrightWhite;
	}
}

NAME_SPACE_END