#pragma once

#include <array>
#include "Declarations.h"

namespace Utils
{
	inline bool isOutOfBounds(unsigned int fromIdx)
	{
		return fromIdx > NUM_PEGS;
	}
};

