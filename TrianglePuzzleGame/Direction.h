#pragma once
#include<string>

enum Direction
{
	//order matches rotation around a z axis
	R, UR, UL, L, DL, DR, NOT_A_DIRECTION
};
Direction convertToDirection(const std::string& direction);


