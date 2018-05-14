#include "Game.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>

#include "Utils.h"
#include"Direction.h"

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
	std::cout << "Pick an index for the starting hole to be." << std::endl;
	int startPos;
	std::cin >> startPos;


	initializeStartHoleTo(startPos);
	while (!gameOver())
	{
		printState();
		requestMove();
	}
}

bool Game::gameOver()
{
	return false;
}

void Game::printState()
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

