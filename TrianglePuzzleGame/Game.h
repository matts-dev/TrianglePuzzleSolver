#pragma once
#include <array>

#include "BoardStructure.h"
#include "Utils.h"

class Game
{
	BoardStructure board;
	std::array<bool, NUM_PEGS> pegsPresent;

public:
	Game();
	~Game();

	void printLocations();
	void initializeStartHoleTo(unsigned int startPos);
	void play();
	bool gameOver();
	void solve();
private:
	void printState();
	void requestMove();
	void commitMove(int from, Direction dir);
	
	int getStartLocationFromUser();
};


