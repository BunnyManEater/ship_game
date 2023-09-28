#include "Game.h"
int main() {
	
	//init srand 
	std::srand(static_cast<unsigned>(time(NULL)));

	//init game engine 
	Game game;

	//Game loop 
	while (game.running()) {

		
		//update
		game.update();

		//render
		game.render();

	}


	return 0;
}

