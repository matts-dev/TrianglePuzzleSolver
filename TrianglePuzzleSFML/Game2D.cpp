#include "Game2D.h"
#include <iostream>
#include <functional>

#include "MathUtils.h"


namespace //translation unit locals
{
	const sf::Vector2f offScreen(-1000, -1000);

	void initializeTextureHelper(sf::Texture& text, std::string path)
	{
		if (!text.loadFromFile(path))
		{
			std::cerr << "failed to load texture " << path << std::endl;
		}
	}

	glm::vec2& directionVectorHelper(IndexedSprite& to, IndexedSprite& from, glm::vec2& storage)
	{
		sf::Vector2f dir_sf = to.getPosition() - from.getPosition();
		storage = glm::normalize(glm::vec2(dir_sf.x, dir_sf.y));
		return storage;
	};

} //end translation unit locals


Game2D::Game2D() :
	//Game(),
	backgroundSprite(),
	pegSprites(15),
	holeSprites(15),
	pegColor(sf::Color::Red),
	selectColor(sf::Color::Blue),
	window(sf::VideoMode(1400, 900), "Peg Game!"),
	mouseManager(window, *this),
	draggingPeg(nullptr),
	currentState(GameState::SELECT_HOLE)
{
	//load textures
	initializeTextureHelper(boardTexture, "assets/background_tri.png");
	initializeTextureHelper(pegTexture, "assets/peg.png");
	initializeTextureHelper(holeTexture, "assets/tri_hole.png");

	//initialize sprites
	backgroundSprite.setTexture(boardTexture);
	for (auto& peg : pegSprites)
	{
		peg.setTexture(pegTexture);
	}
	for (auto& hole : holeSprites)
	{
		hole.setTexture(holeTexture);
	}

	configureBackgroundSprite();
	configurePegHoles();
	configurePegs();
	configureDirectionVectors();
}

Game2D::~Game2D()
{

}

void Game2D::game_loop()
{
	while (window.isOpen())
	{
		io();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		window.clear();
		window.draw(backgroundSprite);
		for (sf::Sprite& hole : holeSprites)
			window.draw(hole);
		for (sf::Sprite& peg : pegSprites)
			window.draw(peg);

		//if user is dragging a peg, it should be drawn on top.
		if (draggingPeg) window.draw(*draggingPeg);

		window.display();
	}
}

void Game2D::requestStateChange(IndexedSprite& peg, IndexedSprite& hole)
{
	glm::vec2 moveDir;
	directionVectorHelper(hole, holeSprites[peg.getIndex()], moveDir);

	for (Direction dir_iter = Direction::R; dir_iter < Direction::NOT_A_DIRECTION; dir_iter = static_cast<Direction>(dir_iter + 1))
	{
		const glm::vec2& dir_v = *directionMap[dir_iter];

		//dot between two normalized vectors, this will be ~1 if they point in the same direction.
		float match = glm::dot(dir_v, moveDir);
		if (MathUtils::almostEqual(match, 1.f, 0.001f))
		{
			//direction match, see if game state allows move.
			commitMove(peg.getIndex(), dir_iter);
			if (gameOver())
			{
				currentState = GameState::GAME_OVER;
			}

			break;
		}
	}
	
}

void Game2D::configureDirectionVectors()
{
	//----------------------
	//           0 
	//	       1   2
	//	     3   4   5
	//	   6   7   8   9
	//	10  11  12  13  14
	// ----------------------
	//We can derive all the direction vectors from the index 4.
	IndexedSprite& s4 = pegSprites[4]; //the center
	directionVectorHelper(pegSprites[5], s4, this->R);
	directionVectorHelper(pegSprites[2], s4, this->UR); //UR
	directionVectorHelper(pegSprites[1], s4, this->UL); //UL
	directionVectorHelper(pegSprites[3], s4, this->L); //L
	directionVectorHelper(pegSprites[7], s4, this->DL); //DL
	directionVectorHelper(pegSprites[8], s4, this->DR); //DR

	directionMap[Direction::R] = &R;
	directionMap[Direction::UR] = &UR;
	directionMap[Direction::UL] = &UL;
	directionMap[Direction::L] = &L;
	directionMap[Direction::DL] = &DL;
	directionMap[Direction::DR] = &DR;
}

/* Refresh peg positions after updates, this keeps indices of pegs matching with underlying layout in game.*/
void Game2D::refreshPegLayout()
{
	using size = std::vector<IndexedSprite>::size_type;

	for (size i = 0; i < pegsPresent.size(); ++i)
	{
		if (pegsPresent[i])
		{
			pegSprites[i].setPosition(holeSprites[i].getPosition());
		}
		else
		{
			pegSprites[i].setPosition(offScreen);
		}
	}
}

void Game2D::configurePegHoles()
{
	using size = std::vector<IndexedSprite>::size_type;
	float scaleXY = 0.8f;

	const sf::Vector2f& center = backgroundSprite.getPosition();
	sf::Vector2f start = center;
	start.y -= backgroundSprite.getGlobalBounds().height / 4;

	//configure a direction to slide the holes in that appears to match the triangle.
	float degrees = 240;
	glm::vec4 init_dir_vec = glm::vec4(1, 0, 0, 0);
	glm::mat4 rotation_matrix;
	rotation_matrix = glm::rotate(rotation_matrix, glm::radians(degrees), glm::vec3(0, 0, 1));
	init_dir_vec = rotation_matrix * init_dir_vec; //column vector multiplication
	init_dir_vec.y *= -1; //sfml y values increase in the down direction
	glm::vec2 verticalDir = glm::normalize(glm::vec2(init_dir_vec.x, init_dir_vec.y));
	glm::vec2 horrizontalDir = glm::normalize(glm::vec2(1, 0));

	int row = 0;
	int eles_in_row = 1;
	int eles_in_current_row = 0;
	//index needed for calculating position
	for (size i = 0; i < holeSprites.size(); ++i)
	{
		IndexedSprite& hole = holeSprites[i];
		hole.setIndex(i);

		sf::FloatRect size = hole.getGlobalBounds();
		hole.setOrigin(size.width / 2, size.height / 2);
		hole.setScale(scaleXY, scaleXY);
		size = hole.getGlobalBounds(); //update to size after scale.


		glm::vec2 offset_vert = verticalDir;
		offset_vert *= size.height; //width could also work since sprites are square
		offset_vert *= row;

		glm::vec2 offset_horr = horrizontalDir;
		offset_horr *= size.width;
		offset_horr *= eles_in_current_row;

		hole.setPosition(start.x + offset_vert.x + offset_horr.x, start.y + offset_vert.y + offset_horr.y);

		eles_in_current_row += 1;
		if (eles_in_current_row == eles_in_row)
		{
			row++;
			eles_in_row += 1;
			eles_in_current_row = 0;
		}
	}
}

void Game2D::configurePegs()
{
	using size = std::vector<IndexedSprite>::size_type;
	float scaleXY = 0.5f;

	//index needed for calculating position
	for (size i = 0; i < pegSprites.size(); ++i)
	{
		IndexedSprite& peg = pegSprites[i];
		IndexedSprite& hole = holeSprites[i];

		peg.setIndex(i);

		sf::FloatRect unscaled_size = peg.getGlobalBounds();
		peg.setOrigin(unscaled_size.width / 2, unscaled_size.height / 2);
		peg.setScale(scaleXY, scaleXY);
		sf::FloatRect size = peg.getGlobalBounds();

		peg.setPosition(hole.getPosition());
		peg.setColor(pegColor);
	}
}

void Game2D::configureBackgroundSprite()
{
	static const float scale = 0.35f;
	sf::FloatRect globalBounds = backgroundSprite.getGlobalBounds();
	float wid = globalBounds.width;
	float hgh = globalBounds.height;
	sf::Vector2u size = window.getSize();

	//setting origin to center allows rotation around the center of the object.
	backgroundSprite.setOrigin(wid / 2.f, hgh / 2.f);
	backgroundSprite.setScale(scale, scale);
	backgroundSprite.setPosition(size.x / 2.f, size.y / 2.f);
}

void Game2D::io()
{
	//mouse input
	mouseManager.io();


	// debug input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		//backgroundSprite.rotate(0.01f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		//backgroundSprite.rotate(-0.01f);
	}
}

// ------------------ mouse manager interface ----------------------------------
void Game2D::mousePressed(sf::Mouse::Button button, float posX, float posY)
{
	if (currentState == GameState::GAME_OVER)
		return;

	if (button == sf::Mouse::Left)
	{
		for (auto& peg : pegSprites)
		{
			if (peg.getGlobalBounds().contains(posX, posY))
			{
				draggingPeg = &peg;
				draggedStartPosition = peg.getPosition();
				break;
			}
		}
	}

	if (currentState == GameState::SELECT_HOLE && draggingPeg)
	{
		currentState = GameState::MOVE_PEGS;
		draggingPeg->setPosition(offScreen);
		initializeStartHoleTo(draggingPeg->getIndex());
		draggingPeg = nullptr;
	}
}

void Game2D::mouseDragged(sf::Mouse::Button button, float posX, float posY)
{
	if (draggingPeg)
	{
		draggingPeg->setPosition(posX, posY);
	}
}

void Game2D::mouseReleased(sf::Mouse::Button button, float posX, float posY)
{
	// Say that a peg is in a hole if it is within the radius of the scaled hole sprite plus the radius of a scaled peg.
	// invariant: holeSprites and pegSprites will be configured an set in a constructor before it is possible that this called.
	static const float PNG_CORRECTION_PEG = 0.80f; //the pngs are not completely filled up wit
	static const float PNG_CORRECTION_HOLE = 0.7f; //the pngs are not completely filled up wit
	static const float THRESHOLD = (PNG_CORRECTION_HOLE * holeSprites[0].getGlobalBounds().width / 2) + (PNG_CORRECTION_PEG * pegSprites[0].getGlobalBounds().width / 2);
	static const float PEG_SIZE = pegSprites[0].getGlobalBounds().width * PNG_CORRECTION_PEG; //if two pegs are not colliding, their centers have to be at least 2 radii away, which is the diameter

	float currentThreshold = THRESHOLD;

	if (draggingPeg)
	{
		IndexedSprite* nearbyHole = nullptr;
		//check if it is near a hole
		for (auto& hole : holeSprites)
		{
			//create a vector from the hole to the peg and get the length;
			const sf::Vector2f& holePos = hole.getPosition();
			glm::vec2 fromHole_v(posX, posY);
			fromHole_v -= glm::vec2(holePos.x, holePos.y);
			float distance = glm::length(fromHole_v);

			if (distance < currentThreshold)
			{
				//find the shortest distance by updating the current threshold
				currentThreshold = distance;
				nearbyHole = &hole;
			}
		}
		
		//check if there is a collision with another peg
		bool collision = false;
		if (nearbyHole)
		{
			const sf::Vector2f& holePos = nearbyHole->getPosition();
			for (auto& peg : pegSprites)
			{
				//it should not be possible to have a collision if within the threshold to a hole

				if (&peg != draggingPeg)
				{


					sf::Vector2f peg_to_hole_v = holePos - peg.getPosition();
					float dist = glm::length(glm::vec2(peg_to_hole_v.x, peg_to_hole_v.y));
					if (dist < PEG_SIZE)
					{
						collision = true;
						break;
					}
				}
			}
		}

		//check that hole is unoccupied
		if (!collision && nearbyHole)
		{
			const sf::Vector2f& holePos = nearbyHole->getPosition();
			//draggingPeg->setPosition(holePos);
			requestStateChange(*draggingPeg, *nearbyHole);
		}
		else
		{
			draggingPeg->setPosition(draggedStartPosition);

		}

		draggingPeg = nullptr;


		refreshPegLayout();
	}
}
// --------------------------------------------------------------------------------
