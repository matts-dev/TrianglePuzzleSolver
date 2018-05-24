#include <SFML/Graphics.hpp>

#include "Game2D.h"

void troubleshooting_msg();

int main()
{
	troubleshooting_msg();

	Game2D game2d;
	game2d.game_loop();
	return 0;
}

void troubleshooting_msg()
{
	std::cout << "Some set up is neccessary if you see a black screen or warning about missing *.dll" << std::endl << std::endl;
	std::cout << "The .exe is usually located in a Debug or Release folder in the solution directory (not project directory)" << std::endl;
	std::cout << "\t .exe locations commonly in the form of:" << std::endl;
	std::cout << "\t \\TrianglePuzzleGame\\TrianglePuzzleGame\\Debug" << std::endl;
	std::cout << "\t \\TrianglePuzzleGame\\TrianglePuzzleGame\\Release" << "\n" << std::endl;

	std::cout << "Solution to Black screen:\n" << "\tMake sure you copy the assets folder to the location of your .exe" << std::endl;
	std::cout << "\t" << "TrianglePuzzleGame\\TrianglePuzzleGame\\TrianglePuzzleSFML\\assets" << "\n" << std::endl;
	

	std::cout << "Solution to missing DLLs:\n\tMake sure you copy all of the .dll files." << std::endl;
	std::cout << "\tThe dll files are located at: TrianglePuzzleGame\\TrianglePuzzleGame\\TrianglePuzzleSFML\\sfml\\lib" << std::endl;
	std::cout << "\tNOTE: you only need to copy the .dll files(do not copy .ilk, .exp, pdb, or any other files)." << std::endl;
	std::cout << "\tHere are the files that need to be moved to your .exe folder:" << std::endl;
	std::cout << "\t\t" << "sfml_audio-2.dll" << std::endl;
	std::cout << "\t\t" << "sfml-audio-d-2.dll" << std::endl;
	std::cout << "\t\t" << "sfml-graphics-2.dll" << std::endl;
	std::cout << "\t\t" << "sfml-graphics-2-d.dll" << std::endl;
	std::cout << "\t\t" << "sfml-network-2.dll" << std::endl;
	std::cout << "\t\t" << "sfml-network-d-2.dll" << std::endl;
	std::cout << "\t\t" << "sfml-system-2.dll" << std::endl;
	std::cout << "\t\t" << "sfml-system-2-d.dll" << std::endl;
	std::cout << "\t\t" << "sfml-window-2.dll" << std::endl;
	std::cout << "\t\t" << "sfml-window-d-2.dll" << std::endl;

	std::cout << "If these steps do not work, the sfml library may need to be re-built for your windows platform." << std::endl;

}