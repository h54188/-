// Example program:
// Using SDL2 to create an application window

#include <stdio.h>
#include "Game.h"
int main(int argc, char* argv[]) {
	Game game;
	bool success = game.Initialize();

	if (success) {
		game.RunLoop();
	}

	game.ShutDown();
	return 0;
}