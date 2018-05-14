#include "BoardStructure.h"
#include "Utils.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PEG NODE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PegNode::PegNode()
{
}

PegNode::~PegNode()
{
}


const PegNode* PegNode::getNeighbor(const Direction direction) const
{
	switch (direction)
	{
		case Direction::R:
			return R;
		case Direction::UR:
			return UR;
		case Direction::UL:
			return UL;
		case Direction::L:
			return L;
		case Direction::DL:
			return DL;
		case Direction::DR:
			return DR;
	}
	return nullptr;
}

inline PegNode * PegNode::getNeighbor(const Direction direction)
{
	const PegNode* const_this = const_cast<const PegNode*>(this);
	return const_cast<PegNode*>(const_this->getNeighbor(direction));
}

unsigned int PegNode::getIndex() const
{
	return assignedIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BOARD STRUCTURE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BoardStructure::BoardStructure() 
	: pegs(15) //use no arg construct to construct 15 pegs.
{
	for (size_t i = 0; i < pegs.size(); ++i)
	{
		pegs[i].assignedIndex = i;
	}

	// positions
	//----------------------
	//           0 
	//	       1   2
	//	     3   4   5
	//	   6   7   8   9
	//	10  11  12  13  14
	// ----------------------
	pegs[0].DL = &pegs[1];
	pegs[0].DR = &pegs[2];

	pegs[1].R = &pegs[2];
	pegs[1].UR = &pegs[0];
	pegs[1].DL = &pegs[3];
	pegs[1].DR = &pegs[4];

	pegs[2].UL = &pegs[0];
	pegs[2].L = &pegs[1];
	pegs[2].DL = &pegs[4];
	pegs[2].DR = &pegs[5];

	pegs[3].R = &pegs[4];
	pegs[3].UR = &pegs[1];
	pegs[3].DL = &pegs[6];
	pegs[3].DR = &pegs[7];

	// positions
	//----------------------
	//           0 
	//	       1   2
	//	     3   4   5
	//	   6   7   8   9
	//	10  11  12  13  14
	// ----------------------
	
	pegs[4].R = &pegs[5];
	pegs[4].UR = &pegs[2];
	pegs[4].UL = &pegs[1];
	pegs[4].L = &pegs[3];
	pegs[4].DL = &pegs[7];
	pegs[4].DR = &pegs[8];

	pegs[5].UL = &pegs[2];
	pegs[5].L = &pegs[4];
	pegs[5].DL = &pegs[8];
	pegs[5].DR = &pegs[9];

	pegs[6].R = &pegs[7];
	pegs[6].UR = &pegs[3];
	pegs[6].DL = &pegs[10];
	pegs[6].DR = &pegs[11];

	pegs[7].R = &pegs[8];
	pegs[7].UR = &pegs[4];
	pegs[7].UL = &pegs[3];
	pegs[7].L = &pegs[6];
	pegs[7].DL = &pegs[11];
	pegs[7].DR = &pegs[12];


	// positions
	//----------------------
	//           0 
	//	       1   2
	//	     3   4   5
	//	   6   7   8   9
	//	10  11  12  13  14
	// ----------------------
	pegs[8].R = &pegs[9];
	pegs[8].UR = &pegs[5];
	pegs[8].UL = &pegs[4];
	pegs[8].L = &pegs[7];
	pegs[8].DL = &pegs[12];
	pegs[8].DR = &pegs[13];

	pegs[9].UL = &pegs[5];
	pegs[9].L = &pegs[8];
	pegs[9].DL = &pegs[13];
	pegs[9].DR = &pegs[14];

	pegs[10].R = &pegs[11];
	pegs[10].UR = &pegs[6];

	pegs[11].R = &pegs[12];
	pegs[11].UR = &pegs[7];
	pegs[11].UL = &pegs[6];
	pegs[11].L = &pegs[10];

	// positions
	//----------------------
	//           0 
	//	       1   2
	//	     3   4   5
	//	   6   7   8   9
	//	10  11  12  13  14
	// ----------------------
	pegs[12].R = &pegs[13];
	pegs[12].UR = &pegs[8];
	pegs[12].UL = &pegs[7];
	pegs[12].L = &pegs[11];

	pegs[13].R = &pegs[14];
	pegs[13].UR = &pegs[9];
	pegs[13].UL = &pegs[8];
	pegs[13].L = &pegs[12];

	pegs[14].UL = &pegs[9];
	pegs[14].L = &pegs[13];

}


BoardStructure::~BoardStructure()
{
}

bool BoardStructure::isValidMove(
	int rawIdx, 
	Direction dir, 
	const std::array<bool, 15>& currentPegs) const
{
	///note that no bounds checking is done on the rawIdx

	//check that there is a peg at the source index.
	if (!currentPegs[rawIdx])
		return false;
	const PegNode* srcPeg = &pegs[rawIdx];
	


	//return when there's no middle, or there's not a peg to jump over.
	const PegNode* middlePeg = srcPeg->getNeighbor(dir);
	if (middlePeg == nullptr || !currentPegs[middlePeg->getIndex()])
		return false;


	//return there's no space, or a peg occupies that space
	const PegNode* dstPeg = middlePeg->getNeighbor(dir);
	if (dstPeg == nullptr || currentPegs[dstPeg->getIndex()])
		return false;

	return true;
}

void BoardStructure::commitMove(int fromIdx, Direction dir, std::array<bool, 15>& currentPegs)
{
	if (!Utils::isOutOfBounds(fromIdx) && isValidMove(fromIdx, dir, currentPegs))
	{
		//below will not be null due to the isValidMove check.
		PegNode& src =  pegs[fromIdx];
		PegNode* mid = src.getNeighbor(dir);
		PegNode* dst = mid->getNeighbor(dir);

		currentPegs[src.assignedIndex] = false;
		currentPegs[mid->assignedIndex] = false;
		currentPegs[dst->assignedIndex] = true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
