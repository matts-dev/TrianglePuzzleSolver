#include "KeyboardManager.h"
#include <stack>



KeyboardManager::KeyboardManager()
{
}


KeyboardManager::~KeyboardManager()
{
}

bool KeyboardManager::keyJustPressed(sf::Keyboard::Key key)
{

	if (pressedKeys.find(key) != pressedKeys.end())
	{
		//key is in the set of previously pressed
		return false;
	}

	if (sf::Keyboard::isKeyPressed(key))
	{
		pressedKeys.insert(key);
		return true;
	}

	return false;
}

/* this method mussed be called within game loop for proper management. */
void KeyboardManager::updateKeyPresses()
{
	using Key = sf::Keyboard::Key;
	static std::vector<Key> low_level_container(20);
	static std::stack<Key, std::vector<Key> > toRemove(low_level_container);

	for (auto itr = pressedKeys.begin(); itr != pressedKeys.end(); ++itr)
	{
		if (!sf::Keyboard::isKeyPressed(*itr))
		{
			toRemove.push(*itr);
		}
	}

	//empty previous remove
	while (!toRemove.empty())
	{
		pressedKeys.erase(toRemove.top());
		toRemove.pop();
	}

}
