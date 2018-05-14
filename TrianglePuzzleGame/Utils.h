#pragma once

#define NUM_PEGS 15

namespace Utils
{
	inline bool isOutOfBounds(unsigned int fromIdx)
	{
		return fromIdx > NUM_PEGS;
	}
};

