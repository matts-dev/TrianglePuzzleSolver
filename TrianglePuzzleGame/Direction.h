#pragma once
#include<string>

enum Direction
{
	UL, UR, R, DR, DL, L, NOT_A_DIRECTION
};
Direction convertToDirection(const std::string& direction);


