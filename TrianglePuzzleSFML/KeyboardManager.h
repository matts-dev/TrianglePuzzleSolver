#pragma once
#include<iostream>
#include<SFML/graphics.hpp>
#include <set>

/* updateKeyPresses must called every game loop iteration to update keymanager state based on key state. */
class KeyboardManager
{
	std::set<sf::Keyboard::Key> pressedKeys;

public:
	KeyboardManager();
	~KeyboardManager();

	bool keyJustPressed(sf::Keyboard::Key key);
	void updateKeyPresses();

};

