#pragma once

#include<SFML/Graphics.hpp>

#include"Game.h" /* super class */
#include"MouseManager.h"
#include"IndexedSprite.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "KeyboardManager.h"
#include <stack>

class Game2D : public Game, public MouseManager::Callback_Interface
{

private: //fields
	//resources
	sf::Texture boardTexture;
	sf::Texture pegTexture;
	sf::Texture holeTexture;

	//sprites
	sf::Sprite backgroundSprite;
	std::vector<IndexedSprite> pegSprites;
	std::vector<IndexedSprite> holeSprites;

	sf::Color pegColor;
	sf::Color selectColor;

	sf::RenderWindow window;
	MouseManager mouseManager;

private: // game state
	enum GameState { SELECT_HOLE, MOVE_PEGS, GAME_OVER };
	GameState currentState;
	IndexedSprite* draggingPeg;
	sf::Vector2f draggedStartPosition;
	KeyboardManager kbManager;
	std::stack<PegArray> previousMove;

private: //helpers
	glm::vec2 R, UR, UL, L, DL, DR;
	std::map<Direction, glm::vec2*> directionMap;

private: //methods
	void configureBackgroundSprite();
	void configurePegHoles();
	void configurePegs();
	void configureDirectionVectors();
	void refreshPegLayout();
	void requestStateChange(IndexedSprite& peg, IndexedSprite& hole);
	void stepTowardSolution();
	void stepBack();

	void io();

	//inherits friend MouseManager; This gives the manager access to the callbacks.
	virtual void mousePressed(sf::Mouse::Button button, float posX, float posY);
	virtual void mouseDragged(sf::Mouse::Button button, float posX, float posY);
	virtual void mouseReleased(sf::Mouse::Button button, float posX, float posY);


public: //methods
	Game2D();
	~Game2D();
	Game2D(const Game2D& copy) = delete;
	Game2D(const Game2D&& copy) = delete;
	const Game2D& operator=(const Game2D& copy) = delete;
	const Game2D& operator=(const Game2D&& copy) = delete;

	void game_loop();
	void reset();
};

