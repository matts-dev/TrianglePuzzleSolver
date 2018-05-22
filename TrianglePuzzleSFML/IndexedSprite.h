#pragma once

#include<SFML/Graphics.hpp>

///wrapper that associates an index with a sprite for convenience 
class IndexedSprite : public sf::Sprite
{
	int index;
public:
	IndexedSprite();;
	void setIndex(int newIndex) { index = newIndex; }
	int getIndex() { return index; }
};

