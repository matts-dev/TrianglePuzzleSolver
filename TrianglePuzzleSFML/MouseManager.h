#pragma once

#include<SFML/Graphics.hpp>

class MouseManager
{
public:
	class Callback_Interface //avoiding struct to keep these methods private
	{
		friend MouseManager; //permit MouseManager access to these call backs.
		virtual void mouseDragged(sf::Mouse::Button button, float posX, float posY) = 0;
		virtual void mousePressed(sf::Mouse::Button button, float posX, float posY) = 0;		
		virtual void mouseReleased(sf::Mouse::Button button, float posX, float posY) = 0;	
	};

private:
	const sf::RenderWindow& context;
	Callback_Interface& registeredCallback;
	bool leftPressed;

public:
	MouseManager(const sf::RenderWindow& context, Callback_Interface& callback);
	~MouseManager();
	void io();
};

