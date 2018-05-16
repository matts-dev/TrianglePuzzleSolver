#pragma once
#include<cstdint>
#include "Utils.h"
#include <array>
#include <memory>


struct StateNode
{
	/*The default ctor and dtor should be good enough for cleaning up resources/links*/

	//requires cpp11 for header file initialization 
	std::shared_ptr<StateNode> previous_state = nullptr;
	uint16_t state;

	void storeState(const std::array<bool, NUM_PEGS>& pegsAvailable);
	void retrieveState(std::array<bool, NUM_PEGS>& pegsAvailable);
	bool isWinningState() const;
	static std::shared_ptr<StateNode> reverseList(std::shared_ptr<StateNode> tail);
};

