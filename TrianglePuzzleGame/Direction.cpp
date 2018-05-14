#include "Direction.h"


Direction convertToDirection(const std::string& direction)
{
	if (direction == "UL")
	{
		return Direction::UL;
	}
	else if (direction == "UR")
	{
		return Direction::UR;
	}
	else if (direction == "R")
	{
		return Direction::R;
	}
	else if (direction == "DR")
	{
		return Direction::DR;
	}
	else if (direction == "DL")
	{
		return Direction::DL;
	}
	else if (direction == "L")
	{
		return Direction::L;
	}
	else
	{
		return Direction::NOT_A_DIRECTION;
	}
}
