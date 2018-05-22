#include "Game.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>

#include "Utils.h"
#include"Direction.h"
#include "StateNode.h"
#include <memory>
#include <queue>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <stack>

Game::Game()
{

}

Game::~Game()
{

}

void Game::printLocations()
{
	int wid = 2;
	int boardCharSize = 10 * wid;
	int index = 0;
	int offset = boardCharSize / (4); //4 is due to dividing by 2, then dividing by width
	int elementsInRow = 1;
	std::string boarder(boardCharSize + wid, '-'); //mult by 2 because set width of 2 modifier used throghout
	std::cout << boarder << std::endl;

	//rows (newlines) are more spaced, so only draw half of the rows.
	for (int row = 0; row < 5; ++row)
	{
		std::cout << std::setw(wid);
		for (int spacer = 0; spacer < offset; ++spacer)
		{
			std::cout << std::setw(wid) << ' ';
		}
		for (int idx = 0; idx < elementsInRow; ++idx)
		{
			std::cout << std::setw(wid) << index++ << std::setw(wid) << " ";
		}
		offset -= 1;
		++elementsInRow;

		std::cout << std::endl;
	}
	std::cout << boarder << std::endl;
}

void Game::initializeStartHoleTo(unsigned int startPos)
{
	for (bool& peg : pegsPresent)
	{
		peg = true;
	}

	if (Utils::isOutOfBounds(startPos))
	{
		throw std::runtime_error("invalid index for starting position");
	}

	pegsPresent[startPos] = false;
}

void Game::play()
{
	printLocations();
	int startPos = getStartLocationFromUser();
	initializeStartHoleTo(startPos);
	while (!gameOver())
	{
		printState();
		requestMove();
	}
	std::cout << "Game Over" << std::endl;
}

bool Game::gameOver()
{
	return !board.hasPossibleMoves(pegsPresent);
}

void Game::solve()
{
	//get the start position
	printLocations();
	int startPos = getStartLocationFromUser();
	initializeStartHoleTo(startPos);

	std::shared_ptr<StateNode> winSequence = solve_from_config();
	
	//print out the sequence to the winning state
	if (winSequence)
	{
		while(winSequence->previous_state != nullptr)
		{
			winSequence->retrieveState(pegsPresent);
			printState();
			std::cout << "Press enter to see the next state" << std::endl << std::endl;
			std::cin.get();

			winSequence = winSequence->previous_state;
		}
		winSequence->retrieveState(pegsPresent);
		printState();
		std::cout << "This is the final state." << std::endl;
	}
	else
	{
		std::cout << "No solution found for 1 peg from this start state." << std::endl;
	}
}

void Game::printState()
{
	int wid = 2;
	int boardCharSize = 10 * wid;
	int index = 0;
	int offset = boardCharSize / (4); //4 is due to dividing by 2, then dividing by width
	int elementsInRow = 1;
	std::string boarder(boardCharSize + wid, '-'); //mult by 2 because set width of 2 modifier used throughout
	std::cout << boarder << std::endl;

	//rows (newlines) are more spaced, so only draw half of the rows.
	for (int row = 0; row < 5; ++row)
	{
		std::cout << std::setw(wid);
		for (int spacer = 0; spacer < offset; ++spacer)
		{
			std::cout << std::setw(wid) << ' ';
		}
		for (int idx = 0; idx < elementsInRow; ++idx)
		{
			if (pegsPresent[index])
				std::cout << std::setw(wid) << index << std::setw(wid) << " ";
			else
				std::cout << std::setw(wid) << "<>" << std::setw(wid) << " ";
			index++;
		}
		offset -= 1;
		++elementsInRow;

		std::cout << std::endl;
	}
	std::cout << boarder << std::endl;
}

void Game::requestMove()
{
	std::cout << "Please enter a peg's index to move:";
	int from;
	std::cin >> from;


	std::cout << "Please enter a direction to move.\n(R=right, UR=upright, UL=upleft, L=left, DL=downleft, DR=downright\n direction:";
	std::string dir;
	std::getline(std::cin, dir); //buffer clear
	std::getline(std::cin, dir);

	//convert string to uppercase
	for (char& c : dir) c = std::toupper(c);

	commitMove(from, convertToDirection(dir));
}


void Game::commitMove(int fromIdx, Direction dir)
{
	board.commitMove(fromIdx, dir, pegsPresent);
}

int Game::getStartLocationFromUser()
{
	std::cout << "Pick an index for the starting hole to be." << std::endl;

	int startPos;
	std::cin >> startPos;
	std::cin.get(); //buffer clear on newline after number.

	return startPos;
}

std::shared_ptr<StateNode> Game::solve_from_config()
{
	using std::chrono::steady_clock;
	using std::chrono::duration_cast;
	using std::chrono::milliseconds;
	using std::chrono::seconds;
	using std::chrono::microseconds;

	std::shared_ptr<StateNode> winSequence = nullptr;

	bool done = false;

	//prepare BFS/DFS
	std::shared_ptr<StateNode> firstState = std::make_shared<StateNode>();
	firstState->storeState(pegsPresent);

	//std::queue<std::shared_ptr<StateNode>> nextStateQueue; //queue is essentialy BFS
	std::stack<std::shared_ptr<StateNode>> nextStateQueue; //stack is essentially DFS
	nextStateQueue.push(firstState);

	//a buffer to read states into
	std::array<bool, NUM_PEGS> pegsSrcBuffer = { 0 };
	std::vector<std::shared_ptr<StateNode>> moveBuffer;


	//hash<uint16_t> is specialized. uint16_t is unsigned short; 
	//this specialization is in the header <functional> http://n.cppreference.com/w/cpp/utility/hash
	std::unordered_set<uint16_t> previouslyVisited;

	steady_clock::time_point end, start = steady_clock::now();

	//BFS using the start state.
	while (nextStateQueue.size() != 0 && !done)
	{
		//std::shared_ptr<StateNode> node = nextStateQueue.front(); //BFS: 237446 microseconds to find solution
		std::shared_ptr<StateNode> node = nextStateQueue.top(); //DFS: 1230 microseconds to find a solution
		nextStateQueue.pop();

		node->retrieveState(pegsSrcBuffer);
		board.getAllMoves(moveBuffer, node);

		for (std::shared_ptr<StateNode>& potentialMove : moveBuffer)
		{
			//std::cout << potentialMove->state << std::endl;
			if (potentialMove->isWinningState())
			{
				//prepare winning state sequence
				winSequence = StateNode::reverseList(potentialMove);
				done = true;
				break;
			}
			else
			{
				//check if this state has been visited
				if (previouslyVisited.find(potentialMove->state) == previouslyVisited.end())
				{
					//state was no in the hashmap.
					previouslyVisited.insert(potentialMove->state);
					nextStateQueue.push(potentialMove);
				}
			}
		}
	}

	end = steady_clock::now();
	steady_clock::duration time = end - start;
	std::cout << "\n Search complete in " << duration_cast<microseconds> (time).count() << " microseconds; press enter.\n" << std::endl;


	return winSequence;
}

