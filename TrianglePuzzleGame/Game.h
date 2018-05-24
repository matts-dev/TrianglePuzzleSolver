#pragma once
#include <array>


#include "BoardStructure.h"
#include "Utils.h"

class Game
{
protected:
	BoardStructure board;
	PegArray pegsPresent;

public:
	Game();
	virtual ~Game();

	void printLocations();
	void initializeStartHoleTo(unsigned int startPos);
	void play();
	bool gameOver();
	void solve();

protected:
	void printState();
	void requestMove();
	bool commitMove(int from, Direction dir);
	
	int getStartLocationFromUser();
	std::shared_ptr<StateNode> solve_from_config();
};


