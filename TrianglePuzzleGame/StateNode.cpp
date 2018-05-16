#include "StateNode.h"
#include <stack>

//typedefs
using arr_sz = std::array<bool, NUM_PEGS>::size_type;

//StateNode::~StateNode()
//{
//	previous_state = nullptr;
//}

void StateNode::storeState(const std::array<bool, NUM_PEGS>& pegsAvailable)
{
	state = 0;
	//for (arr_sz i = 0; i < pegsAvailable.size(); ++i)
	for (auto iter = pegsAvailable.begin(); iter != pegsAvailable.end(); ++iter)
	{
		//bool will be 1 if true, 0 if false. Pack these bits into a state bitvector.
		state <<= 1;
		//state |= static_cast<uint16_t>(pegsAvailable[i]);
		state |= static_cast<uint16_t>(*iter);
	}
}

void StateNode::retrieveState(std::array<bool, NUM_PEGS>& pegsAvailable)
{
	uint16_t mask = 0x0001;
	uint16_t state_cpy = state;

	//loop from end (condition utilizes the fact this is unsigned, so it will become greater than the size of the container on wrap around
	//for (arr_sz i = pegsAvailable.size() - 1; i < pegsAvailable.size(); --i)
	for(auto rter = pegsAvailable.rbegin(); rter != pegsAvailable.rend(); ++rter)
	{
		//bool will be 1 if true, 0 if false. Pack these bits into a state bitvector.
		//pegsAvailable[i] = state_cpy && mask;
		*rter = state_cpy & mask;
		state_cpy >>= 1;
	}

}

bool StateNode::isWinningState() const
{
	int pegCount = 0;	

	uint16_t mask = 0x0001;
	uint16_t state_cpy = state;

	for (int i = 0; i < NUM_PEGS; ++i)
	{
		pegCount += state_cpy & mask;
		state_cpy >>= 1;
	}

	return pegCount == 1;
}

std::shared_ptr<StateNode> StateNode::reverseList(std::shared_ptr<StateNode> tail)
{
	using std::stack;
	using std::shared_ptr;

	stack < shared_ptr<StateNode> > nodeStack;
	shared_ptr<StateNode> iter = tail;
	shared_ptr<StateNode> head = tail;
	shared_ptr<StateNode> lastTop = nullptr;

	//this may be slightly inefficient because of reference count shuffling
	while(iter != nullptr){
		nodeStack.push(iter);

		iter = iter->previous_state;
	}

	head = nodeStack.top();
	lastTop = head;
	if (head)
	{
		nodeStack.pop();
		while (nodeStack.size() != 0)
		{
			//take node off top of stack
			iter = nodeStack.top();
			nodeStack.pop();

			//add to list of nodes
			lastTop->previous_state = iter;
			lastTop = iter;
		}
		//if there is only 1 node (ie head), this will still be safe because (lastTop == head) in that case.
		lastTop->previous_state = nullptr; 
	}

	return head;
}
