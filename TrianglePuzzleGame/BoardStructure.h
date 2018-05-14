#pragma once

#include<vector>
#include<array>

#include "Direction.h"


class PegNode;
class BoardStructure;

class PegNode
{
	//requires cpp11
	PegNode* R = nullptr;
	PegNode* UR = nullptr;
	PegNode* UL = nullptr;
	PegNode* L = nullptr;
	PegNode* DL = nullptr;
	PegNode* DR = nullptr;
	unsigned int assignedIndex = -1;
	
public:
	PegNode();
	~PegNode();

	friend BoardStructure;
	const PegNode* getNeighbor(const Direction direction) const;
	inline PegNode* getNeighbor(const Direction direction);
	unsigned int getIndex() const;
};

/*
	Use this class to manage a pegnode structure so that there are no risks of dangling pointers. 
*/
class BoardStructure
{
	std::vector<PegNode> pegs;
public:
	BoardStructure();
	~BoardStructure();
	bool isValidMove(int rawIdx, Direction dir, const std::array<bool, 15>& currentPegs) const;
	void commitMove(int fromIdx, Direction dir, std::array<bool, 15>& currentPegs);
};

