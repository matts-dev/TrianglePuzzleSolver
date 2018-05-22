#include "MouseManager.h"



MouseManager::MouseManager(const sf::RenderWindow& context, Callback_Interface& callback) :
	context(context),
	registeredCallback(callback),
	leftPressed(false)
{
}

MouseManager::~MouseManager()
{
}

void MouseManager::io()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//conversion constructor provided in Vector2.inl
		sf::Vector2f position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(context));

		if (leftPressed)
		{
			registeredCallback.mouseDragged(sf::Mouse::Left, position.x, position.y);
		}
		else
		{
			leftPressed = true;
			registeredCallback.mousePressed(sf::Mouse::Left, position.x, position.y);
		}
	}
	else
	{
		if (leftPressed)
		{
			sf::Vector2f position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(context));
			leftPressed = false;
			registeredCallback.mouseReleased(sf::Mouse::Left, position.x, position.y);
		}
	}
}
